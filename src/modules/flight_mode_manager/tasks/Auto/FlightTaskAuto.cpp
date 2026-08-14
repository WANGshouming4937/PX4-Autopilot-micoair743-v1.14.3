/****************************************************************************
 *
 *   Copyright (c) 2018-2023 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @file FlightTaskAuto.cpp
 */

#include "FlightTaskAuto.hpp"
#include <mathlib/mathlib.h>
#include <px4_platform_common/events.h>
#include <px4_platform_common/log.h>
#include <systemlib/mavlink_log.h>
#include <float.h>

using namespace matrix;

bool FlightTaskAuto::activate(const trajectory_setpoint_s &last_setpoint)
{
	bool ret = FlightTask::activate(last_setpoint);

	_position_setpoint = _position;
	_velocity_setpoint = _velocity;
	_yaw_setpoint = _yaw;
	_yawspeed_setpoint = 0.0f;

	// Set setpoints equal current state.
	_velocity_setpoint = _velocity;
	_position_setpoint = _position;

	Vector3f vel_prev{last_setpoint.velocity};
	Vector3f pos_prev{last_setpoint.position};
	Vector3f accel_prev{last_setpoint.acceleration};

	for (int i = 0; i < 3; i++) {
		// If the position setpoint is unknown, set to the current position
		if (!PX4_ISFINITE(pos_prev(i))) { pos_prev(i) = _position(i); }

		// If the velocity setpoint is unknown, set to the current velocity
		if (!PX4_ISFINITE(vel_prev(i))) { vel_prev(i) = _velocity(i); }

		// No acceleration estimate available, set to zero if the setpoint is NAN
		if (!PX4_ISFINITE(accel_prev(i))) { accel_prev(i) = 0.f; }
	}

	_position_smoothing.reset(accel_prev, vel_prev, pos_prev);

	_yaw_sp_prev = PX4_ISFINITE(last_setpoint.yaw) ? last_setpoint.yaw : _yaw;
	_updateTrajConstraints();
	_is_emergency_braking_active = false;
	_time_last_cruise_speed_override = 0;
	_airspeed_constraint_state = MissionAirspeedState::Normal;
	_time_last_airspeed_warning = 0;

	return ret;
}

void FlightTaskAuto::reActivate()
{
	FlightTask::reActivate();

	// On ground, reset acceleration and velocity to zero
	_position_smoothing.reset({0.f, 0.f, 0.f}, {0.f, 0.f, 0.7f}, _position);
}

bool FlightTaskAuto::updateInitialize()
{
	bool ret = FlightTask::updateInitialize();

	_sub_home_position.update();
	_sub_vehicle_status.update();
	_sub_triplet_setpoint.update();
	_sub_airspeed_estimator.update();

	// require valid reference and valid target
	ret = ret && _evaluateGlobalReference() && _evaluateTriplets();
	// require valid position
	ret = ret && _position.isAllFinite() && _velocity.isAllFinite();

	return ret;
}

bool FlightTaskAuto::update()
{
	bool ret = FlightTask::update();
	// always reset constraints because they might change depending on the type
	_setDefaultConstraints();

	// The only time a thrust set-point is sent out is during
	// idle. Hence, reset thrust set-point to NAN in case the
	// vehicle exits idle.
	if (_type_previous == WaypointType::idle) {
		_acceleration_setpoint.setNaN();
	}

	// during mission and reposition, raise the landing gears but only
	// if altitude is high enough
	if (_highEnoughForLandingGear()) {
		_gear.landing_gear = landing_gear_s::GEAR_UP;
	}

	switch (_type) {
	case WaypointType::idle:
		// Send zero thrust setpoint
		_position_setpoint.setNaN(); // Don't require any position/velocity setpoints
		_velocity_setpoint.setNaN();
		_acceleration_setpoint = Vector3f(0.f, 0.f, 100.f); // High downwards acceleration to make sure there's no thrust
		break;

	case WaypointType::land:
		_prepareLandSetpoints();
		break;

	case WaypointType::velocity:
		// XY Velocity waypoint
		// TODO : Rewiew that. What is the expected behavior?
		_position_setpoint = Vector3f(NAN, NAN, _position(2));
		_velocity_setpoint.xy() = Vector2f(_velocity).unit_or_zero() * _mc_cruise_speed;
		_velocity_setpoint(2) = NAN;
		break;

	case WaypointType::loiter:
		if (_param_mpc_land_rc_help.get() && _sticks.checkAndUpdateStickInputs()) {
			rcHelpModifyYaw(_yaw_setpoint);
		}

	// FALLTHROUGH
	case WaypointType::takeoff:
	case WaypointType::position:
	default:
		// Simple waypoint navigation: go to xyz target, with standard limitations
		_position_setpoint = _target;
		_velocity_setpoint.setNaN();
		break;
	}

	if (_param_com_obs_avoid.get()) {
		_obstacle_avoidance.updateAvoidanceDesiredSetpoints(_position_setpoint, _velocity_setpoint, (int)_type);
		_obstacle_avoidance.injectAvoidanceSetpoints(_position_setpoint, _velocity_setpoint, _yaw_setpoint,
				_yawspeed_setpoint);
	}

	_checkEmergencyBraking();
	Vector3f waypoints[] = {_prev_wp, _position_setpoint, _next_wp};

	if (isTargetModified()) {
		// In case object avoidance has injected a new setpoint, we take this as the next waypoints
		waypoints[2] = _position_setpoint;
	}

	const bool should_wait_for_yaw_align = _param_mpc_yaw_mode.get() == 4 && !_yaw_sp_aligned;
	const bool force_zero_velocity_setpoint = should_wait_for_yaw_align || _is_emergency_braking_active;

	// Mission airspeed is only used to derive the cruise ground-speed constraint.
	// PositionSmoothing remains responsible for acceleration, jerk, waypoint braking and cornering.
	_updateWindEstimate();
	_updateCruiseSpeed();
	_updateTrajConstraints();
	PositionSmoothing::PositionSmoothingSetpoints smoothed_setpoints;
	_position_smoothing.generateSetpoints(
		_position,
		waypoints,
		_velocity_setpoint,
		_deltatime,
		force_zero_velocity_setpoint,
		smoothed_setpoints
	);

	_jerk_setpoint = smoothed_setpoints.jerk;
	_acceleration_setpoint = smoothed_setpoints.acceleration;
	_velocity_setpoint = smoothed_setpoints.velocity;
	_position_setpoint = smoothed_setpoints.position;

	_unsmoothed_velocity_setpoint = smoothed_setpoints.unsmoothed_velocity;
	_want_takeoff = smoothed_setpoints.unsmoothed_velocity(2) < -0.3f;

	if (!PX4_ISFINITE(_yaw_setpoint) && !PX4_ISFINITE(_yawspeed_setpoint)) {
		// No valid heading -> generate heading in this flight task.
		// MPC_YAW_MODE=5 uses desired airspeed direction; all other generated-heading modes
		// keep the original ground-trajectory behavior.
		const bool heading_generated = (_param_mpc_yaw_mode.get() == 5)
					       ? _generateHeadingAlongAirspeed()
					       : _generateHeadingAlongTraj();

		if (!heading_generated) {
			_yaw_setpoint = PX4_ISFINITE(_yaw_sp_prev) ? _yaw_sp_prev : _yaw;
		}
	}

	// update previous type
	_type_previous = _type;

	// If the FlightTask generates a yaw or a yawrate setpoint that exceeds this value
	// it will see its setpoint constrained here
	_limitYawRate();

	_constraints.want_takeoff = _checkTakeoff();

	return ret;
}

void FlightTaskAuto::overrideCruiseSpeed(const float cruise_speed_m_s)
{
	// Store the raw Mission speed command. Its interpretation (ground speed or airspeed)
	// is selected by MPC_MIS_SPD_MODE in _updateCruiseSpeed().
	_mission_cruise_speed = cruise_speed_m_s;
	_time_last_cruise_speed_override = hrt_absolute_time();
}

void FlightTaskAuto::rcHelpModifyYaw(float &yaw_sp)
{
	// Only set a yawrate setpoint if weather vane is not active or the yaw stick is out of its dead-zone
	if (!_weathervane.isActive() || fabsf(_sticks.getYawExpo()) > FLT_EPSILON) {
		_stick_yaw.generateYawSetpoint(_yawspeed_setpoint, yaw_sp, _sticks.getYawExpo(), _yaw, _is_yaw_good_for_control,
					       _deltatime);

		// Hack to make sure the MPC_YAW_MODE 4 alignment doesn't stop the vehicle from descending when there's yaw input
		_yaw_sp_aligned = true;
	}
}

void FlightTaskAuto::_prepareLandSetpoints()
{
	_velocity_setpoint.setNaN(); // Don't take over any smoothed velocity setpoint

	// Slow down automatic descend close to ground
	float vertical_speed = math::interpolate(_dist_to_ground,
			       _param_mpc_land_alt2.get(), _param_mpc_land_alt1.get(),
			       _param_mpc_land_speed.get(), _param_mpc_z_vel_max_dn.get());

	bool range_dist_available = PX4_ISFINITE(_dist_to_bottom);

	if (range_dist_available && _dist_to_bottom <= _param_mpc_land_alt3.get()) {
		vertical_speed = _param_mpc_land_crawl_speed.get();
	}

	if (_type_previous != WaypointType::land) {
		// initialize yaw and xy-position
		_land_heading = _yaw_setpoint;
		_stick_acceleration_xy.resetPosition(Vector2f(_target(0), _target(1)));
		_initial_land_position = Vector3f(_target(0), _target(1), NAN);
	}

	// Update xy-position in case of landing position changes (etc. precision landing)
	_land_position = Vector3f(_target(0), _target(1), NAN);

	// User input assisted landing
	if (_param_mpc_land_rc_help.get() && _sticks.checkAndUpdateStickInputs()) {
		// Stick full up -1 -> stop, stick full down 1 -> double the speed
		vertical_speed *= (1 - _sticks.getThrottleZeroCenteredExpo());

		rcHelpModifyYaw(_land_heading);

		Vector2f sticks_xy = _sticks.getPitchRollExpo();
		Vector2f sticks_ne = sticks_xy;
		Sticks::rotateIntoHeadingFrameXY(sticks_ne, _yaw, _land_heading);

		const float distance_to_circle = math::trajectory::getMaxDistanceToCircle(_position.xy(), _initial_land_position.xy(),
						 _param_mpc_land_radius.get(), sticks_ne);
		float max_speed;

		if (PX4_ISFINITE(distance_to_circle)) {
			max_speed = math::trajectory::computeMaxSpeedFromDistance(_stick_acceleration_xy.getMaxJerk(),
					_stick_acceleration_xy.getMaxAcceleration(), distance_to_circle, 0.f);

			if (max_speed < 0.5f) {
				sticks_xy.setZero();
			}

		} else {
			max_speed = 0.f;
			sticks_xy.setZero();
		}

		_stick_acceleration_xy.setVelocityConstraint(max_speed);
		_stick_acceleration_xy.generateSetpoints(sticks_xy, _yaw, _land_heading, _position,
				_velocity_setpoint_feedback.xy(), _deltatime);
		_stick_acceleration_xy.getSetpoints(_land_position, _velocity_setpoint, _acceleration_setpoint);

	} else {
		// Make sure we have a valid land position even in the case we loose RC while amending it
		if (!PX4_ISFINITE(_land_position(0))) {
			_land_position.xy() = Vector2f(_position);
		}
	}

	_position_setpoint = _land_position; // The last element of the land position has to stay NAN
	_yaw_setpoint = _land_heading;
	_velocity_setpoint(2) = vertical_speed;
	_gear.landing_gear = landing_gear_s::GEAR_DOWN;
}

void FlightTaskAuto::_limitYawRate()
{
	const float yawrate_max = math::radians(_param_mpc_yawrauto_max.get());

	_yaw_sp_aligned = true;

	if (PX4_ISFINITE(_yaw_setpoint) && PX4_ISFINITE(_yaw_sp_prev)) {
		// Limit the rate of change of the yaw setpoint
		const float dyaw_desired = matrix::wrap_pi(_yaw_setpoint - _yaw_sp_prev);
		const float dyaw_max = yawrate_max * _deltatime;
		const float dyaw = math::constrain(dyaw_desired, -dyaw_max, dyaw_max);
		const float yaw_setpoint_sat = matrix::wrap_pi(_yaw_sp_prev + dyaw);

		// The yaw setpoint is aligned when it is within tolerance
		_yaw_sp_aligned = fabsf(matrix::wrap_pi(_yaw_setpoint - yaw_setpoint_sat)) < math::radians(_param_mis_yaw_err.get());

		_yaw_setpoint = yaw_setpoint_sat;

		if (!PX4_ISFINITE(_yawspeed_setpoint) && (_deltatime > FLT_EPSILON)) {
			// Create a feedforward using the filtered derivative
			_yawspeed_filter.setParameters(_deltatime, .2f);
			_yawspeed_filter.update(dyaw / _deltatime);
			_yawspeed_setpoint = _yawspeed_filter.getState();
		}
	}

	_yaw_sp_prev = PX4_ISFINITE(_yaw_setpoint) ? _yaw_setpoint : _yaw;

	if (PX4_ISFINITE(_yawspeed_setpoint)) {
		// The yaw setpoint is aligned when its rate is not saturated
		_yaw_sp_aligned = _yaw_sp_aligned && (fabsf(_yawspeed_setpoint) < yawrate_max);

		_yawspeed_setpoint = math::constrain(_yawspeed_setpoint, -yawrate_max, yawrate_max);
	}
}

bool FlightTaskAuto::_evaluateTriplets()
{
	// TODO: fix the issues mentioned below
	// We add here some conditions that are only required because:
	// 1. navigator continuously sends triplet during mission due to yaw setpoint. This
	// should be removed in the navigator and only updates if the current setpoint actually has changed.
	//
	// 2. navigator should be responsible to send always three valid setpoints. If there is only one setpoint,
	// then previous will be set to current vehicle position and next will be set equal to setpoint.
	//
	// 3. navigator originally only supports gps guided maneuvers. However, it now also supports some flow-specific features
	// such as land and takeoff. The navigator should use for auto takeoff/land with flow the position in xy at the moment the
	// takeoff/land was initiated. Until then we do this kind of logic here.

	// Check if triplet is valid. There must be at least a valid altitude.

	if (!_sub_triplet_setpoint.get().current.valid || !PX4_ISFINITE(_sub_triplet_setpoint.get().current.alt)) {
		// Best we can do is to just set all waypoints to current state
		_prev_prev_wp = _triplet_prev_wp = _triplet_target = _triplet_next_wp = _position;
		_type = WaypointType::loiter;
		_yaw_setpoint = _yaw;
		_yawspeed_setpoint = NAN;
		_target_acceptance_radius = _sub_triplet_setpoint.get().current.acceptance_radius;
		_updateInternalWaypoints();
		return true;
	}

	_type = (WaypointType)_sub_triplet_setpoint.get().current.type;

	// Keep the raw Mission speed separate from the final ground-speed constraint.
	// MPC_MIS_SPD_MODE=0: raw Mission speed is ground speed (original behavior).
	// MPC_MIS_SPD_MODE=1: raw Mission speed is cruising airspeed in AUTO_MISSION POSITION legs.
	const float cruise_speed_from_triplet = _sub_triplet_setpoint.get().current.cruising_speed;

	if (PX4_ISFINITE(cruise_speed_from_triplet)
	    && (_sub_triplet_setpoint.get().current.timestamp > _time_last_cruise_speed_override)) {
		_mission_cruise_speed = cruise_speed_from_triplet;
	}

	if (!PX4_ISFINITE(_mission_cruise_speed) || (_mission_cruise_speed < FLT_EPSILON)) {
		_mission_cruise_speed = _param_mpc_xy_cruise.get();
	}

	if (_param_mpc_mis_spd_mode.get() == 1) {
		_mc_cruise_airspeed = _mission_cruise_speed;

		// Initialize the ground-speed state without applying the Vg limit to Va.
		if (!PX4_ISFINITE(_mc_cruise_speed)) {
			_mc_cruise_speed = math::min(_mission_cruise_speed, _param_mpc_xy_vel_max.get());
		}

	} else {
		_mc_cruise_airspeed = NAN;
		// Original ground-speed behavior. The maximum-speed constraint is a Vg constraint.
		_mc_cruise_speed = math::min(_mission_cruise_speed, _param_mpc_xy_vel_max.get());
	}

	// Temporary target variable where we save the local reprojection of the latest navigator current triplet.
	Vector3f tmp_target;

	if (!PX4_ISFINITE(_sub_triplet_setpoint.get().current.lat)
	    || !PX4_ISFINITE(_sub_triplet_setpoint.get().current.lon)) {
		// No position provided in xy. Lock position
		if (!_lock_position_xy.isAllFinite()) {
			tmp_target(0) = _lock_position_xy(0) = _position(0);
			tmp_target(1) = _lock_position_xy(1) = _position(1);

		} else {
			tmp_target(0) = _lock_position_xy(0);
			tmp_target(1) = _lock_position_xy(1);
		}

	} else {
		// reset locked position if current lon and lat are valid
		_lock_position_xy.setAll(NAN);

		// Convert from global to local frame.
		_reference_position.project(_sub_triplet_setpoint.get().current.lat, _sub_triplet_setpoint.get().current.lon,
					    tmp_target(0), tmp_target(1));
	}

	tmp_target(2) = -(_sub_triplet_setpoint.get().current.alt - _reference_altitude);

	// Check if anything has changed. We do that by comparing the temporary target
	// to the internal _triplet_target.
	// TODO This is a hack and it would be much better if the navigator only sends out a waypoints once they have changed.

	bool triplet_update = true;
	const bool prev_next_validity_changed = (_prev_was_valid != _sub_triplet_setpoint.get().previous.valid)
						|| (_next_was_valid != _sub_triplet_setpoint.get().next.valid);

	if (_triplet_target.isAllFinite()
	    && fabsf(_triplet_target(0) - tmp_target(0)) < 0.001f
	    && fabsf(_triplet_target(1) - tmp_target(1)) < 0.001f
	    && fabsf(_triplet_target(2) - tmp_target(2)) < 0.001f
	    && !prev_next_validity_changed) {
		// Nothing has changed: just keep old waypoints.
		triplet_update = false;

	} else {
		_triplet_target = tmp_target;
		_target_acceptance_radius = _sub_triplet_setpoint.get().current.acceptance_radius;

		if (!Vector2f(_triplet_target).isAllFinite()) {
			// Horizontal target is not finite.
			_triplet_target(0) = _position(0);
			_triplet_target(1) = _position(1);
		}

		if (!PX4_ISFINITE(_triplet_target(2))) {
			_triplet_target(2) = _position(2);
		}

		// If _triplet_target has updated, update also _triplet_prev_wp and _triplet_next_wp.
		_prev_prev_wp = _triplet_prev_wp;

		if (_isFinite(_sub_triplet_setpoint.get().previous) && _sub_triplet_setpoint.get().previous.valid) {
			_reference_position.project(_sub_triplet_setpoint.get().previous.lat,
						    _sub_triplet_setpoint.get().previous.lon, _triplet_prev_wp(0), _triplet_prev_wp(1));
			_triplet_prev_wp(2) = -(_sub_triplet_setpoint.get().previous.alt - _reference_altitude);

		} else {
			_triplet_prev_wp = _position;
		}

		_prev_was_valid = _sub_triplet_setpoint.get().previous.valid;

		if (_type == WaypointType::loiter) {
			_triplet_next_wp = _triplet_target;

		} else if (_isFinite(_sub_triplet_setpoint.get().next) && _sub_triplet_setpoint.get().next.valid) {
			_reference_position.project(_sub_triplet_setpoint.get().next.lat,
						    _sub_triplet_setpoint.get().next.lon, _triplet_next_wp(0), _triplet_next_wp(1));
			_triplet_next_wp(2) = -(_sub_triplet_setpoint.get().next.alt - _reference_altitude);

		} else {
			_triplet_next_wp = _triplet_target;
		}

		_next_was_valid = _sub_triplet_setpoint.get().next.valid;
	}

	// activation/deactivation of weather vane is based on parameter WV_EN and setting of navigator (allow_weather_vane)
	_weathervane.setNavigatorForceDisabled(_sub_triplet_setpoint.get().current.disable_weather_vane);

	// Calculate the current vehicle state and check if it has updated.
	State previous_state = _current_state;
	_current_state = _getCurrentState();

	if (triplet_update || (_current_state != previous_state) || _current_state == State::offtrack) {
		_updateInternalWaypoints();
	}

	if (_param_com_obs_avoid.get()
	    && _sub_vehicle_status.get().vehicle_type == vehicle_status_s::VEHICLE_TYPE_ROTARY_WING) {
		_obstacle_avoidance.updateAvoidanceDesiredWaypoints(_triplet_target, _yaw_setpoint, _yawspeed_setpoint,
				_triplet_next_wp,
				_sub_triplet_setpoint.get().next.yaw,
				_sub_triplet_setpoint.get().next.yawspeed_valid ? _sub_triplet_setpoint.get().next.yawspeed : (float)NAN,
				_weathervane.isActive(), _sub_triplet_setpoint.get().current.type);
		_obstacle_avoidance.checkAvoidanceProgress(
			_position, _triplet_prev_wp, _target_acceptance_radius, Vector2f(_closest_pt));
	}

	// set heading
	_weathervane.update();

	if (_weathervane.isActive()) {
		_yaw_setpoint = NAN;
		// use the yawrate setpoint from WV only if not moving lateral (velocity setpoint below half of _param_mpc_xy_cruise)
		// otherwise, keep heading constant (as output from WV is not according to wind in this case)
		bool vehicle_is_moving_lateral = _velocity_setpoint.xy().longerThan(_param_mpc_xy_cruise.get() / 2.0f);

		if (vehicle_is_moving_lateral) {
			_yawspeed_setpoint = 0.0f;

		} else {
			_yawspeed_setpoint = _weathervane.getWeathervaneYawrate();
		}

	} else {
		if (!_is_yaw_good_for_control) {
			_yaw_lock = false;
			_yaw_setpoint = NAN;
			_yawspeed_setpoint = 0.f;

		} else if ((_type != WaypointType::takeoff || _sub_triplet_setpoint.get().current.disable_weather_vane)
			   && _sub_triplet_setpoint.get().current.yaw_valid) {
			// Use the yaw computed in Navigator except during takeoff because
			// Navigator is not handling the yaw reset properly.
			// But: use if from Navigator during takeoff if disable_weather_vane is true,
			// because we're then aligning to the transition waypoint.
			// TODO: fix in navigator
			_yaw_setpoint = _sub_triplet_setpoint.get().current.yaw;
			_yawspeed_setpoint = NAN;

		} else {
			_set_heading_from_mode();
		}
	}

	return true;
}

void FlightTaskAuto::_set_heading_from_mode()
{

	Vector2f v; // Vector that points towards desired location

	switch (_param_mpc_yaw_mode.get()) {

	case 0: // Heading points towards the current waypoint.
	case 4: // Same as 0 but yaw first and then go
		v = Vector2f(_target) - Vector2f(_position);
		break;

	case 1: // Heading points towards home.
		if (_sub_home_position.get().valid_lpos) {
			v = Vector2f(&_sub_home_position.get().x) - Vector2f(_position);
		}

		break;

	case 2: // Heading point away from home.
		if (_sub_home_position.get().valid_lpos) {
			v = Vector2f(_position) - Vector2f(&_sub_home_position.get().x);
		}

		break;

	case 3: // Along ground trajectory.
	case 5: // Along desired airspeed direction.
		// These headings depend on generated velocity setpoints and are calculated after
		// PositionSmoothing in _generateHeadingAlongTraj()/_generateHeadingAlongAirspeed().
		v.setAll(NAN);
		break;
	}

	if (v.isAllFinite()) {
		// We only adjust yaw if vehicle is outside of acceptance radius. Once we enter acceptance
		// radius, lock yaw to current yaw.
		// This prevents excessive yawing.
		if (v.longerThan(_target_acceptance_radius)) {
			if (_compute_heading_from_2D_vector(_yaw_setpoint, v)) {
				_yaw_lock = true;
			}
		}

		if (!_yaw_lock) {
			_yaw_setpoint = _yaw;
			_yaw_lock = true;
		}

	} else {
		_yaw_lock = false;
		_yaw_setpoint = NAN;
	}

	_yawspeed_setpoint = NAN;
}

bool FlightTaskAuto::_isFinite(const position_setpoint_s &sp)
{
	return (PX4_ISFINITE(sp.lat) && PX4_ISFINITE(sp.lon) && PX4_ISFINITE(sp.alt));
}

bool FlightTaskAuto::_evaluateGlobalReference()
{
	// check if reference has changed and update.
	// Only update if reference timestamp has changed AND no valid reference altitude
	// is available.
	// TODO: this needs to be revisited and needs a more clear implementation
	if (_sub_vehicle_local_position.get().ref_timestamp == _time_stamp_reference && PX4_ISFINITE(_reference_altitude)) {
		// don't need to update anything
		return true;
	}

	double ref_lat = _sub_vehicle_local_position.get().ref_lat;
	double ref_lon = _sub_vehicle_local_position.get().ref_lon;
	_reference_altitude = _sub_vehicle_local_position.get().ref_alt;

	if (!_sub_vehicle_local_position.get().z_global) {
		// we have no valid global altitude
		// set global reference to local reference
		_reference_altitude = 0.0f;
	}

	if (!_sub_vehicle_local_position.get().xy_global) {
		// we have no valid global alt/lat
		// set global reference to local reference
		ref_lat = 0.0;
		ref_lon = 0.0;
	}

	// init projection
	_reference_position.initReference(ref_lat, ref_lon);

	// check if everything is still finite
	return PX4_ISFINITE(_reference_altitude) && PX4_ISFINITE(ref_lat) && PX4_ISFINITE(ref_lon);
}

State FlightTaskAuto::_getCurrentState()
{
	// Calculate the vehicle current state based on the Navigator triplets and the current position.
	const Vector2f u_prev_to_target_xy = Vector2f(_triplet_target - _triplet_prev_wp).unit_or_zero();
	const Vector2f pos_to_target_xy = Vector2f(_triplet_target - _position);
	const Vector2f prev_to_pos_xy = Vector2f(_position - _triplet_prev_wp);
	// Calculate the closest point to the vehicle position on the line prev_wp - target
	const Vector2f closest_pt_xy = Vector2f(_triplet_prev_wp) + u_prev_to_target_xy * (prev_to_pos_xy *
				       u_prev_to_target_xy);
	_closest_pt = Vector3f(closest_pt_xy(0), closest_pt_xy(1), _triplet_target(2));

	State return_state = State::none;

	if (u_prev_to_target_xy * pos_to_target_xy < 0.0f) {
		// Target is behind
		return_state = State::target_behind;

	} else if (u_prev_to_target_xy * prev_to_pos_xy < 0.0f && prev_to_pos_xy.longerThan(_target_acceptance_radius)) {
		// Previous is in front
		return_state = State::previous_infront;

	} else if (Vector2f(_position - _closest_pt).longerThan(_target_acceptance_radius)) {
		// Vehicle too far from the track
		return_state = State::offtrack;

	}

	return return_state;
}

void FlightTaskAuto::_updateInternalWaypoints()
{
	// The internal Waypoints might differ from _triplet_prev_wp, _triplet_target and _triplet_next_wp.
	// The cases where it differs:
	// 1. The vehicle already passed the target -> go straight to target
	// 2. Previous waypoint is in front of the vehicle -> go straight to previous waypoint
	// 3. The vehicle is far from track -> go straight to closest point on track
	switch (_current_state) {
	case State::target_behind:
		_target = _triplet_target;
		_prev_wp = _position;
		_next_wp = _triplet_next_wp;
		break;

	case State::previous_infront:
		_next_wp = _triplet_target;
		_target = _triplet_prev_wp;
		_prev_wp = _position;
		break;

	case State::offtrack:
		_next_wp = _triplet_target;
		_target = _closest_pt;
		_prev_wp = _position;
		break;

	case State::none:
		_target = _triplet_target;
		_prev_wp = _triplet_prev_wp;
		_next_wp = _triplet_next_wp;
		break;

	default:
		break;

	}
}

bool FlightTaskAuto::_compute_heading_from_2D_vector(float &heading, Vector2f v)
{
	if (PX4_ISFINITE(v.norm_squared()) && v.longerThan(1e-3f)) {
		v.normalize();
		// To find yaw: take dot product of x = (1,0) and v
		// and multiply by the sign given of cross product of x and v.
		// Dot product: (x(0)*v(0)+(x(1)*v(1)) = v(0)
		// Cross product: x(0)*v(1) - v(0)*x(1) = v(1)
		heading = sign(v(1)) * wrap_pi(acosf(v(0)));
		return true;
	}

	// heading unknown and therefore do not change heading
	return false;
}

/**
 * EKF reset handling functions
 * Those functions are called by the base FlightTask in
 * case of an EKF reset event
 */
void FlightTaskAuto::_ekfResetHandlerPositionXY(const matrix::Vector2f &delta_xy)
{
	_position_smoothing.forceSetPosition({_position(0), _position(1), NAN});
}

void FlightTaskAuto::_ekfResetHandlerVelocityXY(const matrix::Vector2f &delta_vxy)
{
	_position_smoothing.forceSetVelocity({_velocity(0), _velocity(1), NAN});
}

void FlightTaskAuto::_ekfResetHandlerPositionZ(float delta_z)
{
	_position_smoothing.forceSetPosition({NAN, NAN, _position(2)});
}

void FlightTaskAuto::_ekfResetHandlerVelocityZ(float delta_vz)
{
	_position_smoothing.forceSetVelocity({NAN, NAN, _velocity(2)});
}

void FlightTaskAuto::_ekfResetHandlerHeading(float delta_psi)
{
	_yaw_sp_prev += delta_psi;
}

void FlightTaskAuto::_checkEmergencyBraking()
{
	if (!_is_emergency_braking_active) {
		// activate emergency braking if significantly outside of velocity bounds
		const float factor = 1.3f;
		const bool is_vertical_speed_exceeded = _position_smoothing.getCurrentVelocityZ() >
							(factor * _param_mpc_z_vel_max_dn.get())
							|| _position_smoothing.getCurrentVelocityZ() < -(factor * _param_mpc_z_vel_max_up.get());
		const bool is_horizontal_speed_exceeded = _position_smoothing.getCurrentVelocityXY().longerThan(
					factor * _param_mpc_xy_vel_max.get());

		if (is_vertical_speed_exceeded || is_horizontal_speed_exceeded) {
			_is_emergency_braking_active = true;
		}

	} else {
		// deactivate emergency braking when the vehicle has come to a full stop
		if (_position_smoothing.getCurrentVelocityZ() < 0.01f
		    && _position_smoothing.getCurrentVelocityZ() > -0.01f
		    && !_position_smoothing.getCurrentVelocityXY().longerThan(0.01f)) {
			_is_emergency_braking_active = false;
		}
	}
}

bool FlightTaskAuto::_generateHeadingAlongTraj()
{
	bool res = false;
	Vector2f vel_sp_xy(_velocity_setpoint);
	Vector2f traj_to_target = Vector2f(_target) - Vector2f(_position);

	if ((vel_sp_xy.longerThan(2.0f)) &&
	    (traj_to_target.longerThan(2.f))) {
		// Generate heading from velocity vector, only if it is long enough
		// and if the drone is far enough from the target
		_compute_heading_from_2D_vector(_yaw_setpoint, vel_sp_xy);
		res = true;
	}

	return res;
}

bool FlightTaskAuto::_generateHeadingAlongAirspeed()
{
	bool res = false;
	const Vector2f ground_velocity_sp(_velocity_setpoint);
	const Vector2f air_velocity_sp = ground_velocity_sp - _wind_velocity_xy;
	const Vector2f traj_to_target = Vector2f(_target) - Vector2f(_position);

	if (air_velocity_sp.longerThan(2.0f) && traj_to_target.longerThan(2.f)) {
		_compute_heading_from_2D_vector(_yaw_setpoint, air_velocity_sp);
		res = true;
	}

	return res;
}

void FlightTaskAuto::_updateWindEstimate()
{
	const airspeed_estimator_s &wind_est = _sub_airspeed_estimator.get();
	const hrt_abstime now = hrt_absolute_time();

	const bool timestamp_valid = wind_est.timestamp > 0 && now >= wind_est.timestamp;
	const bool fresh = timestamp_valid && (now - wind_est.timestamp <= WIND_ESTIMATE_TIMEOUT_US);
	const bool wind_finite = PX4_ISFINITE(wind_est.v_wind_est[0]) && PX4_ISFINITE(wind_est.v_wind_est[1]);
	const bool airspeed_finite = PX4_ISFINITE(wind_est.airspeed_est);

	// update_enable is the estimator-level validity gate. ekf_update and algebraic_valid are
	// intentionally not used as cycle-by-cycle gates because either may legitimately be false
	// on a cycle without meaning that the retained wind state is invalid.
	const bool estimator_valid = fresh && wind_finite && airspeed_finite && (wind_est.update_enable > 0.5f);
	_wind_estimate_valid = estimator_valid;

	// Follow a finite, fresh estimator output even while update_enable is false. This allows an
	// estimator that decays its state towards zero to cooperate with the validity blend below.
	if (fresh && wind_finite) {
		_wind_velocity_xy_raw = Vector2f(wind_est.v_wind_est[0], wind_est.v_wind_est[1]);
	}

	const float blend_target = estimator_valid ? 1.f : 0.f;
	const float alpha = (_deltatime > FLT_EPSILON)
			    ? math::constrain(_deltatime / (WIND_VALIDITY_TAU_S + _deltatime), 0.f, 1.f)
			    : 1.f;

	_wind_valid_blend += alpha * (blend_target - _wind_valid_blend);
	_wind_valid_blend = math::constrain(_wind_valid_blend, 0.f, 1.f);
	_wind_velocity_xy = _wind_velocity_xy_raw * _wind_valid_blend;
}

float FlightTaskAuto::_airspeedToGroundspeed(float airspeed_sp, const Vector2f &track_direction,
		MissionAirspeedState &state) const
{
	if (!PX4_ISFINITE(airspeed_sp) || airspeed_sp <= FLT_EPSILON
	    || !track_direction.isAllFinite() || !track_direction.longerThan(1e-3f)
	    || !_wind_velocity_xy.isAllFinite()) {
		return airspeed_sp;
	}

	const Vector2f track_unit = track_direction.unit_or_zero();
	const float wind_parallel = _wind_velocity_xy * track_unit;
	const Vector2f wind_perpendicular = _wind_velocity_xy - wind_parallel * track_unit;
	const float wind_perpendicular_sq = wind_perpendicular.norm_squared();
	const float wind_speed = _wind_velocity_xy.norm();
	const float airspeed_max = math::max(_param_mpc_mis_aspd_max.get(), 0.1f);
	const float ground_speed_max = math::max(_param_mpc_xy_vel_max.get(), 0.f);
	const float ground_speed_min = math::min(MIN_GROUND_PROGRESS_SPEED, ground_speed_max);

	// The minimum airspeed achievable while preserving the current ground-track direction
	// and respecting the usable ground-speed interval [Vg_min, Vg_max].
	const float ground_speed_min_air = math::constrain(wind_parallel, ground_speed_min, ground_speed_max);
	const float track_min_airspeed_sq = (ground_speed_min_air - wind_parallel)
					       * (ground_speed_min_air - wind_parallel) + wind_perpendicular_sq;
	const float track_min_airspeed = sqrtf(math::max(track_min_airspeed_sq, 0.f));

	// Even if the track were abandoned, the minimum possible airspeed under the ground-speed
	// limit is max(|wind|-Vg_max, 0). In V1 both infeasible states are warning-only: the
	// Mission track is retained and we use the best-effort ground speed on that track.
	if (_wind_estimate_valid) {
		const float globally_min_airspeed = math::max(wind_speed - ground_speed_max, 0.f);

		if (globally_min_airspeed > airspeed_max + 1e-3f) {
			state = MissionAirspeedState::FullyInfeasible;

		} else if (track_min_airspeed > airspeed_max + 1e-3f) {
			state = MissionAirspeedState::TrackInfeasible;
		}
	}

	const float radicand = airspeed_sp * airspeed_sp - wind_perpendicular_sq;

	if (PX4_ISFINITE(radicand) && radicand >= 0.f) {
		const float root = sqrtf(radicand);
		const float ground_speed_high = wind_parallel + root;
		const float ground_speed_low = wind_parallel - root;

		// Prefer the larger positive-progress solution. If it violates Vg_max, the second
		// root can still satisfy the same airspeed exactly (typical in a strong tailwind).
		if (PX4_ISFINITE(ground_speed_high)
		    && ground_speed_high >= ground_speed_min
		    && ground_speed_high <= ground_speed_max) {
			return ground_speed_high;
		}

		if (PX4_ISFINITE(ground_speed_low)
		    && ground_speed_low >= ground_speed_min
		    && ground_speed_low <= ground_speed_max) {
			return ground_speed_low;
		}
	}

	// The requested cruise airspeed cannot be matched exactly while preserving both the
	// current track and the Vg bounds. If the safety envelope itself is still feasible this
	// is CruiseLimited. For infeasible states keep their higher-severity classification.
	if (state == MissionAirspeedState::Normal) {
		state = MissionAirspeedState::CruiseLimited;
	}

	// In an infeasible state V1 keeps the original Mission track and chooses the allowed
	// ground speed that minimizes actual airspeed. It only warns; it does not enter drift mode.
	if (state == MissionAirspeedState::TrackInfeasible
	    || state == MissionAirspeedState::FullyInfeasible) {
		return ground_speed_min_air;
	}

	// CruiseLimited but still inside the safety envelope: choose the feasible Vg that makes
	// actual airspeed as close as possible to the constrained cruise-air-speed command.
	const float airspeed_at_vg_min = sqrtf((ground_speed_min - wind_parallel) * (ground_speed_min - wind_parallel)
					     + wind_perpendicular_sq);
	const float airspeed_at_vg_max = sqrtf((ground_speed_max - wind_parallel) * (ground_speed_max - wind_parallel)
					     + wind_perpendicular_sq);

	if (airspeed_sp < track_min_airspeed) {
		return ground_speed_min_air;
	}

	return (fabsf(airspeed_at_vg_max - airspeed_sp) <= fabsf(airspeed_at_vg_min - airspeed_sp))
	       ? ground_speed_max : ground_speed_min;
}

void FlightTaskAuto::_updateCruiseSpeed()
{
	float mission_speed = _mission_cruise_speed;

	if (!PX4_ISFINITE(mission_speed) || mission_speed < FLT_EPSILON) {
		mission_speed = _param_mpc_xy_cruise.get();
	}

	const float original_ground_speed = math::min(mission_speed, _param_mpc_xy_vel_max.get());
	const bool in_auto_mission = _sub_vehicle_status.get().nav_state == vehicle_status_s::NAVIGATION_STATE_AUTO_MISSION;
	const bool next_is_land = _sub_triplet_setpoint.get().next.valid
				  && (_sub_triplet_setpoint.get().next.type == position_setpoint_s::SETPOINT_TYPE_LAND);
	const bool use_airspeed_cruise = (_param_mpc_mis_spd_mode.get() == 1)
					 && in_auto_mission
					 && (_type == WaypointType::position)
					 && !next_is_land
					 && (_current_state == State::none)
					 && !_is_emergency_braking_active
					 && !isTargetModified();

	if (!use_airspeed_cruise) {
		// Takeoff, Land, move-to-land approach, Loiter, off-track recovery, obstacle-modified
		// targets and non-Mission Auto modes keep the original ground-speed semantics.
		_mc_cruise_airspeed = NAN;
		_mc_cruise_speed = original_ground_speed;
		_updateAirspeedConstraintWarning(MissionAirspeedState::Normal);
		return;
	}

	const float airspeed_max = math::max(_param_mpc_mis_aspd_max.get(), 0.1f);
	_mc_cruise_airspeed = math::min(mission_speed, airspeed_max);

	MissionAirspeedState new_state = (mission_speed > airspeed_max + 1e-3f)
					 ? MissionAirspeedState::CruiseLimited
					 : MissionAirspeedState::Normal;

	Vector2f track_direction;
	track_direction.setAll(NAN);

	// Use the previous cycle's unsmoothed trajectory direction when available. It follows
	// corner smoothing better than yaw or a raw waypoint bearing. Fall back to target direction.
	const Vector2f unsmoothed_velocity_xy(_unsmoothed_velocity_setpoint);

	if (unsmoothed_velocity_xy.isAllFinite() && unsmoothed_velocity_xy.longerThan(.2f)) {
		track_direction = unsmoothed_velocity_xy.unit_or_zero();

	} else {
		const Vector2f to_target = Vector2f(_target) - Vector2f(_position);

		if (to_target.isAllFinite() && to_target.longerThan(.2f)) {
			track_direction = to_target.unit_or_zero();
		}
	}

	float ground_speed_target = _mc_cruise_airspeed;

	if (track_direction.isAllFinite()) {
		ground_speed_target = _airspeedToGroundspeed(_mc_cruise_airspeed, track_direction, new_state);
	}

	// MPC_XY_VEL_MAX is deliberately applied after Va -> Vg conversion: it constrains Vg, not Va.
	const float ground_speed_max = math::max(_param_mpc_xy_vel_max.get(), 0.f);
	const float ground_speed_min = math::min(MIN_GROUND_PROGRESS_SPEED, ground_speed_max);
	const float ground_speed_limited = math::constrain(ground_speed_target, ground_speed_min, ground_speed_max);

	if (fabsf(ground_speed_limited - ground_speed_target) > 1e-3f
	    && new_state == MissionAirspeedState::Normal) {
		new_state = MissionAirspeedState::CruiseLimited;
	}

	ground_speed_target = ground_speed_limited;
	_updateAirspeedConstraintWarning(new_state);

	// Filter only the airspeed-derived cruise constraint. The actual trajectory remains subject to
	// PositionSmoothing acceleration/jerk/braking/corner limits, so airspeed is a cruise policy,
	// not a hard instantaneous speed command.
	if (!PX4_ISFINITE(_mc_cruise_speed)) {
		_mc_cruise_speed = ground_speed_target;

	} else {
		const float alpha = (_deltatime > FLT_EPSILON)
				    ? math::constrain(_deltatime / (CRUISE_SPEED_TAU_S + _deltatime), 0.f, 1.f)
				    : 1.f;
		_mc_cruise_speed += alpha * (ground_speed_target - _mc_cruise_speed);
	}

	_mc_cruise_speed = math::constrain(_mc_cruise_speed, ground_speed_min, ground_speed_max);
}

void FlightTaskAuto::_updateAirspeedConstraintWarning(MissionAirspeedState new_state)
{
	const bool infeasible = (new_state == MissionAirspeedState::TrackInfeasible)
				|| (new_state == MissionAirspeedState::FullyInfeasible);
	const hrt_abstime now = hrt_absolute_time();
	const bool state_changed = new_state != _airspeed_constraint_state;
	const bool repeat_due = infeasible && (_time_last_airspeed_warning == 0
					       || now - _time_last_airspeed_warning >= AIRSPEED_WARNING_REPEAT_US);

	if (infeasible && (state_changed || repeat_due)) {
		const float wind_speed = _wind_velocity_xy.norm();
		const float airspeed_max = _param_mpc_mis_aspd_max.get();

		if (new_state == MissionAirspeedState::FullyInfeasible) {
			mavlink_log_critical(
				&_mavlink_log_pub,
				"Wind exceeds speed capability; continue mission\t");
			events::send(
				events::ID("flight_task_auto_airspeed_fully_infeasible"),
				events::Log::Critical,
				"Wind exceeds airspeed/groundspeed capability; continuing mission");
			PX4_ERR("Airspeed limits infeasible W=%.1f VaMax=%.1f",
				(double)wind_speed,
				(double)airspeed_max);
		} else {
			mavlink_log_warning(
				&_mavlink_log_pub,
				"Mission track infeasible due to wind\t");
			events::send(
				events::ID("flight_task_auto_airspeed_track_infeasible"),
				events::Log::Warning,
				"Mission track infeasible due to wind; continuing mission");
			PX4_WARN("Airspeed track infeasible W=%.1f VaMax=%.1f",
				(double)wind_speed,
				(double)airspeed_max);
			}

		_time_last_airspeed_warning = now;
	}

	if (!infeasible) {
		_time_last_airspeed_warning = 0;
	}

	_airspeed_constraint_state = new_state;
}

bool FlightTaskAuto::isTargetModified() const
{
	const bool xy_modified = (_target - _position_setpoint).xy().longerThan(FLT_EPSILON);
	const bool z_valid = PX4_ISFINITE(_position_setpoint(2));
	const bool z_modified =  z_valid && std::fabs((_target - _position_setpoint)(2)) > FLT_EPSILON;

	return xy_modified || z_modified;
}

void FlightTaskAuto::_updateTrajConstraints()
{
	// update params of the position smoothing
	_position_smoothing.setMaxAllowedHorizontalError(_param_mpc_xy_err_max.get());
	_position_smoothing.setVerticalAcceptanceRadius(_param_nav_mc_alt_rad.get());
	_position_smoothing.setCruiseSpeed(_mc_cruise_speed);
	_position_smoothing.setHorizontalTrajectoryGain(_param_mpc_xy_traj_p.get());
	_position_smoothing.setTargetAcceptanceRadius(_target_acceptance_radius);

	// Update the constraints of the trajectories
	_position_smoothing.setMaxAccelerationXY(_param_mpc_acc_hor.get()); // TODO : Should be computed using heading
	_position_smoothing.setMaxVelocityXY(_param_mpc_xy_vel_max.get());
	float max_jerk = _param_mpc_jerk_auto.get();
	_position_smoothing.setMaxJerk({max_jerk, max_jerk, max_jerk}); // TODO : Should be computed using heading

	if (_is_emergency_braking_active) {
		// When initializing with large velocity, allow 1g of
		// acceleration in 1s on all axes for fast braking
		_position_smoothing.setMaxAcceleration({9.81f, 9.81f, 9.81f});
		_position_smoothing.setMaxJerk({9.81f, 9.81f, 9.81f});

		// If the current velocity is beyond the usual constraints, tell
		// the controller to exceptionally increase its saturations to avoid
		// cutting out the feedforward
		_constraints.speed_down = math::max(fabsf(_position_smoothing.getCurrentVelocityZ()), _constraints.speed_down);
		_constraints.speed_up = math::max(fabsf(_position_smoothing.getCurrentVelocityZ()), _constraints.speed_up);

	} else if (_unsmoothed_velocity_setpoint(2) < 0.f) { // up
		float z_accel_constraint = _param_mpc_acc_up_max.get();
		float z_vel_constraint = _param_mpc_z_v_auto_up.get();

		// The constraints are broken because they are used as hard limits by the position controller, so put this here
		// until the constraints don't do things like cause controller integrators to saturate. Once the controller
		// doesn't use z speed constraints, this can go in _prepareTakeoffSetpoints(). Accel limit is to
		// emulate the motor ramp (also done in the controller) so that the controller can actually track the setpoint.
		if (_type == WaypointType::takeoff &&  _dist_to_ground < _param_mpc_land_alt1.get()) {
			z_vel_constraint = _param_mpc_tko_speed.get();
			z_accel_constraint = math::min(z_accel_constraint, _param_mpc_tko_speed.get() / _param_mpc_tko_ramp_t.get());

			// Keep the altitude setpoint at the current altitude
			// to avoid having it going down into the ground during
			// the initial ramp as the velocity does not start at 0
			_position_smoothing.forceSetPosition({NAN, NAN, _position(2)});
		}

		_position_smoothing.setMaxVelocityZ(z_vel_constraint);
		_position_smoothing.setMaxAccelerationZ(z_accel_constraint);

	} else { // down
		_position_smoothing.setMaxAccelerationZ(_param_mpc_acc_down_max.get());
		_position_smoothing.setMaxVelocityZ(_param_mpc_z_v_auto_dn.get());
	}

	// Stretch the constraints of the velocity controller to leave some room for an additional
	// correction required by the altitude/vertical position controller
	_constraints.speed_down = math::max(_constraints.speed_down, 1.2f * _param_mpc_z_v_auto_dn.get());;
	_constraints.speed_up = math::max(_constraints.speed_up, 1.2f * _param_mpc_z_v_auto_up.get());;
}

bool FlightTaskAuto::_highEnoughForLandingGear()
{
	// return true if altitude is above two meters
	return _dist_to_ground > 2.0f;
}

void FlightTaskAuto::updateParams()
{
	FlightTask::updateParams();

	// make sure that alt1 is above alt2
	_param_mpc_land_alt1.set(math::max(_param_mpc_land_alt1.get(), _param_mpc_land_alt2.get()));
}
