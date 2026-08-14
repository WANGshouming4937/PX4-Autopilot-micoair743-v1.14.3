/****************************************************************************
 *
 *   Copyright (c) 2018-2019 PX4 Development Team. All rights reserved.
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
 * @file FlightTaskOrbit.cpp
 */

#include "FlightTaskOrbit.hpp"

#include <lib/systemlib/mavlink_log.h>
#include <mathlib/mathlib.h>
#include <px4_platform_common/events.h>
#include <px4_platform_common/log.h>
#include <lib/geo/geo.h>

using namespace matrix;

FlightTaskOrbit::FlightTaskOrbit()
{
	_sticks_data_required = false;
}

bool FlightTaskOrbit::applyCommandParameters(const vehicle_command_s &command, bool &success)
{
	if (command.command != vehicle_command_s::VEHICLE_CMD_DO_ORBIT) {
		return false;
	}

	success = true;
	// save previous velocity and rotation direction
	bool new_is_clockwise = _orbit_speed_command >= 0.f;
	float new_radius = _orbit_radius;
	float new_absolute_velocity = fabsf(_orbit_speed_command);

	// commanded radius
	if (PX4_ISFINITE(command.param1)) {
		// Note: Radius sign is defined as orbit direction in MAVLINK
		const float radius_parameter = command.param1;
		new_is_clockwise = radius_parameter > 0;
		new_radius = fabsf(radius_parameter);
	}

	//wsh edited
	// commanded velocity, take sign of radius as rotation direction
	if (PX4_ISFINITE(command.param2) && fabsf(command.param2) > FLT_EPSILON){
		new_absolute_velocity = fabsf(command.param2);
	} else {
		// if param2 is zero, use default cruise speed
		 _param_mc_orbit_vel.update();
        	new_absolute_velocity = _param_mc_orbit_vel.get();
	}


	float new_velocity = signFromBool(new_is_clockwise) * new_absolute_velocity;

	if (math::isInRange(new_radius, _radius_min, _param_mc_orbit_rad_max.get())) {
		_started_clockwise = new_is_clockwise;
		_sanitizeParams(new_radius, new_velocity);
		_orbit_radius = new_radius;
		_orbit_speed_command = new_velocity;

		// In ground-speed mode keep the original PX4 semantics directly.
		// In airspeed mode _orbit_velocity is recomputed from wind every cycle.
		if (_param_mpc_mis_spd_mode.get() == 0) {
			_orbit_velocity = _orbit_speed_command;
		}

	} else {
		mavlink_log_critical(&_mavlink_log_pub, "Orbit radius limit exceeded\t");
		events::send(events::ID("orbit_radius_exceeded"), events::Log::Alert, "Orbit radius limit exceeded");
		success = false;
	}

	//wsh edited
	// commanded heading behaviour
	// if (PX4_ISFINITE(command.param3)) {
	// 	_yaw_behaviour = command.param3;
	// }
	// 处理航向 (Yaw Behaviour) - 这里引入新参数 MC_ORBIT_YAW
    	_param_mc_orbit_yaw.update(); // 更新参数
	_yaw_behaviour = _param_mc_orbit_yaw.get();

	// save current yaw estimate for ORBIT_YAW_BEHAVIOUR_HOLD_INITIAL_HEADING
	_initial_heading = _yaw;

	// commanded center coordinates
	if (PX4_ISFINITE(command.param5) && PX4_ISFINITE(command.param6)) {
		if (_geo_projection.isInitialized()) {
			_center.xy() = _geo_projection.project(command.param5, command.param6);

		} else {
			success = false;
		}
	}

	// commanded altitude
	if (PX4_ISFINITE(command.param7)) {
		if (_geo_projection.isInitialized()) {
			_center(2) = _global_local_alt0 - command.param7;

		} else {
			success = false;
		}
	}

	// perpendicularly approach the orbit circle again when new parameters get commanded
	if (!_is_position_on_circle() && !_in_circle_approach) {
		_in_circle_approach = true;
		_circle_approach_state = CircleApproachState::AlignToApproachPoint;
		_position_smoothing.reset(_acceleration_setpoint, _velocity_setpoint, _position);
	}

	return true;
}

bool FlightTaskOrbit::sendTelemetry()
{
	orbit_status_s orbit_status{};
	orbit_status.radius = math::signNoZero(_orbit_speed_command) * _orbit_radius;
	orbit_status.frame = 0; // MAV_FRAME::MAV_FRAME_GLOBAL
	orbit_status.yaw_behaviour = _yaw_behaviour;

	if (_geo_projection.isInitialized()) {
		// local -> global
		_geo_projection.reproject(_center(0), _center(1), orbit_status.x, orbit_status.y);
		orbit_status.z = _global_local_alt0 - _position_setpoint(2);

	} else {
		return false; // don't send the message if the transformation failed
	}

	orbit_status.timestamp = hrt_absolute_time();
	_orbit_status_pub.publish(orbit_status);

	return true;
}

void FlightTaskOrbit::_sanitizeParams(float &radius, float &velocity) const
{
	// clip the radius to be within range
	radius = math::constrain(radius, _radius_min, _param_mc_orbit_rad_max.get());

	if (_param_mpc_mis_spd_mode.get() == 1) {
		// In airspeed mode velocity is the commanded airspeed magnitude here.
		// Centripetal acceleration cannot be constrained with Va because a_c = Vg_t^2 / R.
		// The final ground tangential speed is limited in _airspeedToOrbitGroundspeed().
		const float airspeed_max = math::max(_param_mpc_mis_aspd_max.get(), 0.1f);
		velocity = math::constrain(velocity, -airspeed_max, airspeed_max);

	} else {
		// Original local Orbit ground-speed behaviour.
		velocity = math::constrain(velocity, -fabsf(_param_mpc_xy_cruise.get()), fabsf(_param_mpc_xy_cruise.get()));

		const float acceleration_max = math::max(_param_mc_orbit_acc_max.get(), 0.f);
		const bool exceeds_maximum_acceleration = (velocity * velocity) >= acceleration_max * radius;

		if (exceeds_maximum_acceleration) {
			velocity = sign(velocity) * sqrtf(acceleration_max * radius);
		}
	}
}

bool FlightTaskOrbit::activate(const trajectory_setpoint_s &last_setpoint)
{
	bool ret = FlightTaskManualAltitude::activate(last_setpoint);
	_orbit_radius = _radius_min;
	_orbit_speed_command = 1.f;
	_orbit_velocity = 1.f;
	_center = _position;
	_initial_heading = _yaw;
	_slew_rate_yaw.setForcedValue(_yaw);
	_slew_rate_yaw.setSlewRate(math::radians(_param_mpc_yawrauto_max.get()));
	_wind_velocity_xy_raw.zero();
	_wind_velocity_xy.zero();
	_wind_valid_blend = 0.f;
	_wind_estimate_valid = false;
	_airspeed_constraint_state = MissionAirspeedState::Normal;
	_time_last_airspeed_warning = 0;

	// need a valid position and velocity
	ret = ret && _position.isAllFinite() && _velocity.isAllFinite();

	Vector3f pos_prev{last_setpoint.position};
	Vector3f vel_prev{last_setpoint.velocity};
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
	_in_circle_approach = true;
	_circle_approach_state = CircleApproachState::AlignToApproachPoint;

	return ret;
}

bool FlightTaskOrbit::update()
{
	bool ret = true;

	// Wind is needed both by airspeed speed control and MPC_YAW_MODE=5.
	_updateWindEstimate();

	_updateTrajectoryBoundaries();
	_adjustParametersByStick();

	if (_in_circle_approach) {
		// Circle approach keeps the original ground-speed PositionSmoothing behaviour.
		// MPC_YAW_MODE=5 is handled inside the approach state machine where appropriate.
		_generate_circle_approach_setpoints();
		_updateAirspeedConstraintWarning(MissionAirspeedState::Normal);

	} else {
		// update altitude
		ret = ret && FlightTaskManualAltitudeSmoothVel::update();

		// Generate x/y setpoints first. In airspeed mode this computes the final ground speed.
		_generate_circle_setpoints();

		if (_param_mpc_yaw_mode.get() == 5) {
			// Airspeed direction has priority over the Orbit yaw behaviour.
			// If it cannot be generated, fall back to the configured MC_ORBIT_YAW behaviour.
			if (!_setYawAlongAirspeed(Vector2f(_velocity_setpoint))) {
				_generate_circle_yaw_setpoints();
			}

		} else {
			_generate_circle_yaw_setpoints();
		}
	}

	// Apply yaw smoothing
	_yaw_setpoint = _slew_rate_yaw.update(_yaw_setpoint, _deltatime);

	// publish information to UI
	sendTelemetry();

	return ret;
}

void FlightTaskOrbit::_updateTrajectoryBoundaries()
{
	// update params of the position smoothing
	_position_smoothing.setMaxAllowedHorizontalError(_param_mpc_xy_err_max.get());
	_position_smoothing.setVerticalAcceptanceRadius(_param_nav_mc_alt_rad.get());
	_position_smoothing.setCruiseSpeed(_param_mpc_xy_cruise.get());
	_position_smoothing.setHorizontalTrajectoryGain(_param_mpc_xy_traj_p.get());
	_position_smoothing.setTargetAcceptanceRadius(_horizontal_acceptance_radius);

	// Update the constraints of the trajectories
	_position_smoothing.setMaxAccelerationXY(_param_mpc_acc_hor.get()); // TODO : Should be computed using heading
	_position_smoothing.setMaxVelocityXY(_param_mpc_xy_vel_max.get());
	float max_jerk = _param_mpc_jerk_auto.get();
	_position_smoothing.setMaxJerk({max_jerk, max_jerk, max_jerk}); // TODO : Should be computed using heading

	if (_velocity_setpoint(2) < 0.f) { // up
		_position_smoothing.setMaxVelocityZ(_param_mpc_z_v_auto_up.get());
		_position_smoothing.setMaxAccelerationZ(_param_mpc_acc_up_max.get());

	} else { // down
		_position_smoothing.setMaxAccelerationZ(_param_mpc_acc_down_max.get());
		_position_smoothing.setMaxVelocityZ(_param_mpc_z_v_auto_dn.get());
	}

}

bool FlightTaskOrbit::_is_position_on_circle() const
{
	return _is_position_on_circle_xy(_horizontal_acceptance_radius)
	       && fabsf(_position(2) - _center(2)) < _param_nav_mc_alt_rad.get();

}

bool FlightTaskOrbit::_is_position_on_circle_xy(float acceptance_radius) const
{
	return fabsf(Vector2f(_position - _center).length() - _orbit_radius) < acceptance_radius;
}

bool FlightTaskOrbit::_is_yaw_reached(float yaw_setpoint) const
{
	const float yaw_error = atan2f(sinf(yaw_setpoint - _yaw), cosf(yaw_setpoint - _yaw));
	return fabsf(yaw_error) < math::radians(5.f);
}

void FlightTaskOrbit::_adjustParametersByStick()
{
	float radius = _orbit_radius;
	float velocity = _orbit_speed_command;

	switch (_yaw_behaviour) {
	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TANGENT_TO_CIRCLE:
		radius -= signFromBool(_started_clockwise) * _sticks.getPositionExpo()(1) * _deltatime * _param_mpc_xy_cruise.get();
		velocity += signFromBool(_started_clockwise) * _sticks.getPositionExpo()(0) * _deltatime * _param_mpc_acc_hor.get();
		break;

	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_INITIAL_HEADING:
	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_UNCONTROLLED:
	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_RC_CONTROLLED:
	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TO_CIRCLE_CENTER:
	default:
		// stick input adjusts parameters within a fixed time frame
		radius -= _sticks.getPositionExpo()(0) * _deltatime * _param_mpc_xy_cruise.get();
		velocity -= _sticks.getPositionExpo()(1) * _deltatime * _param_mpc_acc_hor.get();
		break;
	}

	_sanitizeParams(radius, velocity);
	_orbit_radius = radius;
	_orbit_speed_command = velocity;

	if (_param_mpc_mis_spd_mode.get() == 0) {
		_orbit_velocity = _orbit_speed_command;
	}
}

void FlightTaskOrbit::_generate_circle_approach_setpoints()
{
	const Vector2f center2d = Vector2f(_center);
	const Vector2f center_to_position_xy = Vector2f(_position) - center2d;
	Vector2f closest_point_on_circle = center2d + center_to_position_xy.unit_or_zero() * _orbit_radius;

	if (center_to_position_xy.norm() <= FLT_EPSILON) {
		closest_point_on_circle = Vector2f(_position) + Vector2f(cosf(_yaw), sinf(_yaw)) * _orbit_radius;
	}

	const Vector3f target_circle_point{closest_point_on_circle(0), closest_point_on_circle(1), _center(2)};
	const Vector2f position_to_circle_point = closest_point_on_circle - Vector2f(_position);
	float approach_yaw_setpoint = _yaw;

	if (position_to_circle_point.norm() > FLT_EPSILON) {
		approach_yaw_setpoint = atan2f(position_to_circle_point(1), position_to_circle_point(0));
	}

	Vector3f center_to_position = _position - _center;
	Vector2f tangent_direction{-center_to_position(1), center_to_position(0)};
	tangent_direction = tangent_direction.unit_or_zero() * math::signNoZero(_orbit_speed_command);

	float tangent_yaw_setpoint = _yaw;

	if (tangent_direction.norm() > FLT_EPSILON) {
		tangent_yaw_setpoint = atan2f(tangent_direction(1), tangent_direction(0));
	}

	_yawspeed_setpoint = NAN;

	if (_circle_approach_state == CircleApproachState::AlignToApproachPoint) {
		_yaw_setpoint = approach_yaw_setpoint;
		_position_setpoint = _position;
		_velocity_setpoint.zero();
		_acceleration_setpoint.zero();
		_jerk_setpoint.zero();

		if (_is_yaw_reached(_yaw_setpoint)) {
			_circle_approach_state = CircleApproachState::ApproachCircle;
			_position_smoothing.reset(_acceleration_setpoint, _velocity_setpoint, _position);
		}

	} else if (_circle_approach_state == CircleApproachState::ApproachCircle) {
		PositionSmoothing::PositionSmoothingSetpoints out_setpoints;
		_position_smoothing.generateSetpoints(_position, target_circle_point,
						       {0.f, 0.f, 0.f}, _deltatime, false, out_setpoints);

		_yaw_setpoint = approach_yaw_setpoint;
		_position_setpoint = out_setpoints.position;
		_velocity_setpoint = out_setpoints.velocity;
		_acceleration_setpoint = out_setpoints.acceleration;
		_jerk_setpoint = out_setpoints.jerk;

		if (_param_mpc_yaw_mode.get() == 5) {
			// During the moving part of the approach, point along the desired airspeed.
			// If the velocity is too small, keep the original approach-point yaw.
			_setYawAlongAirspeed(Vector2f(_velocity_setpoint));
		}

		if (_is_position_on_circle_xy(_horizontal_acceptance_radius)) {
			_circle_approach_state = CircleApproachState::AlignTangent;
		}

	} else {
		if (!_is_position_on_circle_xy(_horizontal_acceptance_radius_exit)) {
			_circle_approach_state = CircleApproachState::ApproachCircle;
			_position_smoothing.reset(_acceleration_setpoint, _velocity_setpoint, _position);
			_yaw_setpoint = approach_yaw_setpoint;
			_position_setpoint = _position;
			_velocity_setpoint.zero();
			_acceleration_setpoint.zero();
			_jerk_setpoint.zero();
			return;
		}

		PositionSmoothing::PositionSmoothingSetpoints out_setpoints;
		_position_smoothing.generateSetpoints(_position, target_circle_point,
						       {0.f, 0.f, 0.f}, _deltatime, false, out_setpoints);

		_yaw_setpoint = tangent_yaw_setpoint;

		if (_param_mpc_yaw_mode.get() == 5) {
			// Align with the air-relative direction expected when entering the circle.
			float tangent_ground_speed = fabsf(_orbit_speed_command);

			if (_param_mpc_mis_spd_mode.get() == 1) {
				MissionAirspeedState tmp_state = MissionAirspeedState::Normal;
				const float airspeed_sp = math::min(fabsf(_orbit_speed_command),
								   math::max(_param_mpc_mis_aspd_max.get(), 0.1f));
				tangent_ground_speed = _airspeedToOrbitGroundspeed(airspeed_sp, tangent_direction,
										    Vector2f{0.f, 0.f}, tmp_state);
			}

			_setYawAlongAirspeed(tangent_direction * tangent_ground_speed);
		}

		_position_setpoint = out_setpoints.position;
		_velocity_setpoint = out_setpoints.velocity;
		_acceleration_setpoint = out_setpoints.acceleration;
		_jerk_setpoint = out_setpoints.jerk;

		if (_is_position_on_circle() && _is_yaw_reached(_yaw_setpoint)) {
			_in_circle_approach = false;
			_circle_approach_state = CircleApproachState::AlignToApproachPoint;
			FlightTaskManualAltitudeSmoothVel::_smoothing.reset(
				PX4_ISFINITE(_acceleration_setpoint(2)) ? _acceleration_setpoint(2) : 0.f,
				PX4_ISFINITE(_velocity_setpoint(2)) ? _velocity_setpoint(2) : _velocity(2),
				PX4_ISFINITE(_position_setpoint(2)) ? _position_setpoint(2) : _position(2));
		}
	}
}

void FlightTaskOrbit::_generate_circle_setpoints()
{
	const Vector3f center_to_position = _position - _center;
	const Vector2f center_to_position_xy(center_to_position);
	const Vector2f radial_direction = center_to_position_xy.unit_or_zero();

	// Positive tangent direction follows the original positive _orbit_velocity convention.
	Vector2f tangent_direction(-center_to_position(1), center_to_position(0));
	tangent_direction = tangent_direction.unit_or_zero();

	// Radial velocity correction used to stay on the commanded circle.
	const Vector2f radial_velocity =
		(_orbit_radius - center_to_position_xy.norm()) * radial_direction;

	if (_param_mpc_mis_spd_mode.get() == 1) {
		const float command_sign = math::signNoZero(_orbit_speed_command);
		const Vector2f track_direction = tangent_direction * command_sign;
		const float airspeed_max = math::max(_param_mpc_mis_aspd_max.get(), 0.1f);
		const float airspeed_sp = math::min(fabsf(_orbit_speed_command), airspeed_max);

		MissionAirspeedState state = MissionAirspeedState::Normal;
		const float tangential_ground_speed =
			_airspeedToOrbitGroundspeed(airspeed_sp, track_direction, radial_velocity, state);

		// _orbit_velocity always remains the signed FINAL tangential GROUND speed.
		_orbit_velocity = command_sign * tangential_ground_speed;
		_velocity_setpoint.xy() = track_direction * tangential_ground_speed + radial_velocity;

		_updateAirspeedConstraintWarning(state);

	} else {
		// Original ground-speed Orbit behaviour.
		_orbit_velocity = _orbit_speed_command;
		_velocity_setpoint.xy() = tangent_direction * _orbit_velocity + radial_velocity;
		_updateAirspeedConstraintWarning(MissionAirspeedState::Normal);
	}

	_position_setpoint(0) = _position_setpoint(1) = NAN;

	// Centripetal acceleration uses the final tangential ground speed.
	const float radius = math::max(_orbit_radius, _radius_min);
	_acceleration_setpoint.xy() =
		-radial_direction * _orbit_velocity * _orbit_velocity / radius;
}

void FlightTaskOrbit::_updateWindEstimate()
{
	_sub_airspeed_estimator.update();

	const airspeed_estimator_s &wind_est = _sub_airspeed_estimator.get();
	const hrt_abstime now = hrt_absolute_time();

	const bool timestamp_valid = wind_est.timestamp > 0 && now >= wind_est.timestamp;
	const bool fresh = timestamp_valid && (now - wind_est.timestamp <= WIND_ESTIMATE_TIMEOUT_US);
	const bool wind_finite = PX4_ISFINITE(wind_est.v_wind_est[0]) && PX4_ISFINITE(wind_est.v_wind_est[1]);
	const bool airspeed_finite = PX4_ISFINITE(wind_est.airspeed_est);

	// Keep the same estimator-validity semantics as FlightTaskAuto.
	const bool estimator_valid = fresh && wind_finite && airspeed_finite && (wind_est.update_enable > 0.5f);
	_wind_estimate_valid = estimator_valid;

	// Follow a finite, fresh estimator output even while update_enable is false,
	// allowing the estimator's own wind decay-to-zero logic to cooperate with this blend.
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

float FlightTaskOrbit::_airspeedToOrbitGroundspeed(float airspeed_sp,
		const Vector2f &track_direction,
		const Vector2f &radial_velocity,
		MissionAirspeedState &state) const
{
	if (!PX4_ISFINITE(airspeed_sp) || airspeed_sp <= FLT_EPSILON
	    || !track_direction.isAllFinite() || !track_direction.longerThan(1e-3f)
	    || !radial_velocity.isAllFinite() || !_wind_velocity_xy.isAllFinite()) {
		return math::max(airspeed_sp, 0.f);
	}

	const Vector2f track_unit = track_direction.unit_or_zero();
	const float ground_speed_max = math::max(_param_mpc_xy_vel_max.get(), 0.f);
	const float acceleration_max = math::max(_param_mc_orbit_acc_max.get(), 0.f);
	const float radius = math::max(_orbit_radius, _radius_min);

	// Radial correction is orthogonal to the ideal tangent direction. Reserve the required
	// ground-speed magnitude for the radial correction first.
	const float radial_speed_sq = radial_velocity.norm_squared();
	float tangential_max_from_ground = 0.f;

	if (radial_speed_sq < ground_speed_max * ground_speed_max) {
		tangential_max_from_ground =
			sqrtf(math::max(ground_speed_max * ground_speed_max - radial_speed_sq, 0.f));
	}

	// The centripetal acceleration constraint is a_c = Vg_t^2 / R.
	const float tangential_max_from_accel = sqrtf(math::max(acceleration_max * radius, 0.f));
	const float tangential_max = math::min(tangential_max_from_ground, tangential_max_from_accel);
	const float tangential_min = math::min(MIN_GROUND_PROGRESS_SPEED, tangential_max);

	// va = vt * t + vr - wind = vt * t + q
	const Vector2f q = radial_velocity - _wind_velocity_xy;
	const float q_parallel = q * track_unit;
	const Vector2f q_perpendicular = q - q_parallel * track_unit;
	const float q_perpendicular_sq = q_perpendicular.norm_squared();

	// Find the minimum airspeed achievable while retaining the current Orbit ground-track direction.
	const float best_tangential_speed =
		(tangential_max > FLT_EPSILON)
		? math::constrain(-q_parallel, tangential_min, tangential_max)
		: 0.f;

	const Vector2f track_min_air_velocity = best_tangential_speed * track_unit + q;
	const float track_min_airspeed = track_min_air_velocity.norm();

	const float airspeed_max = math::max(_param_mpc_mis_aspd_max.get(), 0.1f);

	// Warning-only infeasibility classification. V1 never abandons the Orbit track.
	if (_wind_estimate_valid) {
		// Even if the track were abandoned, any ground velocity is limited by MPC_XY_VEL_MAX.
		const float globally_min_airspeed =
			math::max(_wind_velocity_xy.norm() - ground_speed_max, 0.f);

		if (globally_min_airspeed > airspeed_max + AIRSPEED_FEASIBILITY_EPS) {
			state = MissionAirspeedState::FullyInfeasible;

		} else if (track_min_airspeed > airspeed_max + AIRSPEED_FEASIBILITY_EPS) {
			state = MissionAirspeedState::TrackInfeasible;
		}
	}

	const float radicand = airspeed_sp * airspeed_sp - q_perpendicular_sq;

	if (PX4_ISFINITE(radicand) && radicand >= 0.f) {
		const float root = sqrtf(radicand);
		const float tangential_high = -q_parallel + root;
		const float tangential_low = -q_parallel - root;

		// Prefer the larger positive-progress solution. Under a strong tailwind the second
		// root may still satisfy the same Va exactly while respecting Vg/acceleration limits.
		if (PX4_ISFINITE(tangential_high)
		    && tangential_high >= tangential_min
		    && tangential_high <= tangential_max) {
			return tangential_high;
		}

		if (PX4_ISFINITE(tangential_low)
		    && tangential_low >= tangential_min
		    && tangential_low <= tangential_max) {
			if (state == MissionAirspeedState::Normal) {
				state = MissionAirspeedState::CruiseLimited;
			}

			return tangential_low;
		}
	}

	if (state == MissionAirspeedState::Normal) {
		state = MissionAirspeedState::CruiseLimited;
	}

	// For TrackInfeasible/FullyInfeasible V1 only warns and keeps the original Orbit track.
	// Use the admissible tangential speed that minimizes the achieved airspeed on that track.
	if (state == MissionAirspeedState::TrackInfeasible
	    || state == MissionAirspeedState::FullyInfeasible) {
		return best_tangential_speed;
	}

	// CruiseLimited but still inside the maximum-airspeed envelope: choose the boundary
	// that gives an achieved airspeed closest to the constrained Va command.
	if (tangential_max <= FLT_EPSILON) {
		return 0.f;
	}

	const Vector2f air_velocity_at_min = tangential_min * track_unit + q;
	const Vector2f air_velocity_at_max = tangential_max * track_unit + q;

	const float airspeed_at_min = air_velocity_at_min.norm();
	const float airspeed_at_max = air_velocity_at_max.norm();

	if (airspeed_sp < track_min_airspeed) {
		return best_tangential_speed;
	}

	return (fabsf(airspeed_at_max - airspeed_sp) <= fabsf(airspeed_at_min - airspeed_sp))
	       ? tangential_max : tangential_min;
}

bool FlightTaskOrbit::_setYawAlongAirspeed(const Vector2f &ground_velocity_sp)
{
	if (!ground_velocity_sp.isAllFinite()) {
		return false;
	}

	const Vector2f air_velocity_sp = ground_velocity_sp - _wind_velocity_xy;

	if (!air_velocity_sp.isAllFinite() || !air_velocity_sp.longerThan(0.1f)) {
		return false;
	}

	_yaw_setpoint = atan2f(air_velocity_sp(1), air_velocity_sp(0));
	_yawspeed_setpoint = NAN;
	return true;
}

void FlightTaskOrbit::_updateAirspeedConstraintWarning(MissionAirspeedState new_state)
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
			mavlink_log_critical(&_mavlink_log_pub,
					     "Orbit airspeed limits infeasible\t");

			events::send(events::ID("orbit_airspeed_fully_infeasible"),
				     events::Log::Critical,
				     "Orbit airspeed/groundspeed limits infeasible; continuing orbit");

			PX4_ERR("Orbit airspeed limits infeasible W=%.1f VaMax=%.1f",
				(double)wind_speed, (double)airspeed_max);

		} else {
			mavlink_log_warning(&_mavlink_log_pub,
					    "Orbit track infeasible due to wind\t");

			events::send(events::ID("orbit_airspeed_track_infeasible"),
				     events::Log::Warning,
				     "Orbit track infeasible due to wind; continuing orbit");

			PX4_WARN("Orbit track infeasible W=%.1f VaMax=%.1f",
				 (double)wind_speed, (double)airspeed_max);
		}

		_time_last_airspeed_warning = now;
	}

	if (!infeasible) {
		_time_last_airspeed_warning = 0;
	}

	_airspeed_constraint_state = new_state;
}

void FlightTaskOrbit::_generate_circle_yaw_setpoints()
{
	Vector3f center_to_position = _position - _center;

	switch (_yaw_behaviour) {
	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_INITIAL_HEADING:
		// make vehicle keep the same heading as when the orbit was commanded
		_yaw_setpoint = _initial_heading;
		_yawspeed_setpoint = NAN;
		break;

	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_UNCONTROLLED:
		// no yaw setpoint
		_yaw_setpoint = NAN;
		_yawspeed_setpoint = NAN;
		break;

	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TANGENT_TO_CIRCLE:
		_yaw_setpoint = atan2f(math::signNoZero(_orbit_velocity) * center_to_position(0),
				       -math::signNoZero(_orbit_velocity) * center_to_position(1));
		_yawspeed_setpoint = _orbit_velocity / _orbit_radius;
		break;

	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_RC_CONTROLLED:
		// inherit setpoint from altitude flight task
		break;

	case orbit_status_s::ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TO_CIRCLE_CENTER:
	default:
		_yaw_setpoint = atan2f(-center_to_position(1), -center_to_position(0));
		// yawspeed feed-forward because we know the necessary angular rate
		_yawspeed_setpoint = _orbit_velocity / _orbit_radius;
		break;
	}
}
