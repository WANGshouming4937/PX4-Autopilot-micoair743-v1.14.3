/****************************************************************************
 *
 *   Copyright (c) 2017 PX4 Development Team. All rights reserved.
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
 * @file precland.cpp
 *
 * Helper class to do precision landing with a landing target
 *
 * @author Nicolas de Palezieux (Sunflower Labs) <ndepal@gmail.com>
 */

#include "precland.h"
#include "navigator.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>

#include <systemlib/err.h>
#include <systemlib/mavlink_log.h>

#include <uORB/uORB.h>
#include <uORB/topics/position_setpoint_triplet.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/vehicle_command.h>

#define SEC2USEC 1000000.0f

#define STATE_TIMEOUT 10000000 // [us] Maximum time to spend in any state

static constexpr const char *LOST_TARGET_ERROR_MESSAGE = "Lost landing target while landing";
static constexpr float YAW_DEG_TO_RAD = 0.017453292519943295f;
static float wrap_yaw(float angle) { return atan2f(sinf(angle), cosf(angle)); }

PrecLand::PrecLand(Navigator *navigator) :
	MissionBlock(navigator),
	ModuleParams(navigator)
{
	_handle_param_acceleration_hor = param_find("MPC_ACC_HOR");
	_handle_param_xy_vel_cruise = param_find("MPC_XY_CRUISE");

	updateParams();
}

void
PrecLand::on_activation()
{
	_state = PrecLandState::Start;
	_search_cnt = 0;
	_last_slewrate_time = 0;
	// ly_vision_landing: 每次激活重新初始化自定义末段、有效目标缓存、暂停及yaw所有权，避免继承上一轮着陆状态。
	_descent_gate.reset();
	_target_pose = {};
	_target_pose_valid = false;
	_target_pose_updated = false;
	_horizontal_alignment_last_check = 0;
	_last_valid_target_xy = matrix::Vector2f(NAN, NAN);
	_committed_xy = matrix::Vector2f(NAN, NAN);
	_timeout_hold_xy = matrix::Vector2f(NAN, NAN);
	_hold_alt_valid = false;
	_timeout_hold_valid = false;
	_hold_alt = NAN;
	_gate_height = NAN;
	_height_source = 0;
	_hold_alt_reset_counter = _navigator->get_global_position()->alt_reset_counter;
	_xy_reset_counter = _navigator->get_local_position()->xy_reset_counter;
	_yaw_reference = NAN;
	_committed_yaw = NAN;
	_heading_rate = NAN;
	_previous_heading = NAN;
	_yaw_reference_time = 0;
	_previous_heading_time = 0;
	_heading_reset_counter = _navigator->get_local_position()->heading_reset_counter;
	_saved_weather_vane_disabled = _navigator->get_position_setpoint_triplet()->current.disable_weather_vane;
	_navigator->get_position_setpoint_triplet()->current.precision_yaw_control = false;
	_navigator->get_position_setpoint_triplet()->current.precision_descent_pause = false;

	vehicle_local_position_s *vehicle_local_position = _navigator->get_local_position();

	_map_ref.initReference(vehicle_local_position->ref_lat, vehicle_local_position->ref_lon);

	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();

	pos_sp_triplet->next.valid = false;
	pos_sp_triplet->previous.valid = false;

	// Check that the current position setpoint is valid, otherwise land at current position
	if (!pos_sp_triplet->current.valid) {
		PX4_WARN("Reset");
		pos_sp_triplet->current.lat = _navigator->get_global_position()->lat;
		pos_sp_triplet->current.lon = _navigator->get_global_position()->lon;
		pos_sp_triplet->current.alt = _navigator->get_global_position()->alt;
		pos_sp_triplet->current.valid = true;
		pos_sp_triplet->current.timestamp = hrt_absolute_time();
	}

	_sp_pev = matrix::Vector2f(0, 0);
	_sp_pev_prev = matrix::Vector2f(0, 0);
	_last_slewrate_time = 0;

	switch_to_state_start();

	_is_activated = true;
}

void
PrecLand::on_active()
{
	update_reference_resets();
	// get new target measurement
	_target_pose_updated = _target_pose_sub.update(&_target_pose);

	if (_target_pose_updated) {
		_target_pose_valid = true;
	}

	if ((hrt_elapsed_time(&_target_pose.timestamp) / 1e6f) > _param_pld_btout.get()) {
		_target_pose_valid = false;
	}

	if (target_usable(_param_pld_btout.get())) {
		_last_valid_target_xy = matrix::Vector2f(_target_pose.x_abs, _target_pose.y_abs);
	}

	// stop if we are landed
	if (_navigator->get_land_detected()->landed) {
		switch_to_state_done();
	}

	switch (_state) {
	case PrecLandState::Start:
		run_state_start();
		break;

	case PrecLandState::HorizontalApproach:
		run_state_horizontal_approach();
		break;

	case PrecLandState::DescendAboveTarget:
		run_state_descend_above_target();
		break;

	case PrecLandState::FinalApproach:
		run_state_final_approach();
		break;

	case PrecLandState::Search:
		run_state_search();
		break;

	case PrecLandState::Fallback:
		run_state_fallback();
		break;

	case PrecLandState::Done:
		// nothing to do
		break;

	default:
		// unknown state
		break;
	}

}

void
PrecLand::on_inactivation()
{
	_is_activated = false;
	auto *triplet = _navigator->get_position_setpoint_triplet();
	// ly_vision_landing: 离开精准着陆时撤销本项目暂停/yaw标志并通知下游；不把暂停或视觉航向带到其他模式。
	const bool descent_pause_was_active = triplet->current.precision_descent_pause;
	triplet->current.precision_descent_pause = false;
	if (triplet->current.precision_yaw_control) {
		triplet->current.precision_yaw_control = false;
		triplet->current.disable_weather_vane = _saved_weather_vane_disabled;
		_navigator->set_position_setpoint_triplet_updated();

	} else if (descent_pause_was_active) {
		_navigator->set_position_setpoint_triplet_updated();
	}
	_descent_gate.reset();
	_hold_alt_valid = false;
	_timeout_hold_valid = false;
	publish_diagnostic_status(true);
}

void
// ly_vision_landing: 新增只读阶段快照：通常10 Hz，状态变化立即发，记录目标/命令/门控/yaw；它不是控制指令话题。
PrecLand::publish_diagnostic_status(bool force)
{
	const uint64_t now = hrt_absolute_time();
	const uint8_t gate_reason = static_cast<uint8_t>(_descent_gate.reason());
	if (!force && _state == _last_diagnostic_state && gate_reason == _last_gate_reason
	    && now - _last_diagnostic_time < 100000) { return; }
	_last_diagnostic_time = now;
	_last_diagnostic_state = _state;
	_last_gate_reason = gate_reason;
	precision_landing_status_s report{};
	report.timestamp = now;
	report.state = static_cast<uint8_t>(_state);
	report.mode = static_cast<uint8_t>(_mode);
	report.active = _is_activated;
	report.target_abs_valid = _target_pose.abs_pos_valid;
	report.landed = _navigator->get_land_detected()->landed;
	const auto *position = _navigator->get_local_position();
	report.horizontal_error = target_usable(fminf(_param_pld_btout.get(), 0.5f))
		? hypotf(_target_pose.x_abs - position->x, _target_pose.y_abs - position->y) : NAN;
	report.xy_gate_enabled = _param_pld_xy_gate.get() != 0;
	report.xy_gate_active = _descent_gate.active();
	report.descent_paused = _descent_gate.paused();
	report.xy_gate_timed_out = _descent_gate.timedOut();
	report.touchdown_committed = _descent_gate.committed();
	report.pause_reason = gate_reason;
	report.surface_distance = landing_surface_distance(report.height_source);
	report.horizontal_speed = position->v_xy_valid ? hypotf(position->vx, position->vy) : NAN;
	report.aligned_time_s = _descent_gate.stableTime(now);
	report.pause_time_s = _descent_gate.pauseTime(now);
	report.yaw_enabled = _param_pld_yaw_en.get() != 0;
	report.yaw_valid = yaw_usable();
	const float desired_yaw = report.yaw_valid ? wrap_yaw(_target_pose.yaw_abs + _param_pld_yaw_off.get() * YAW_DEG_TO_RAD) : NAN;
	report.yaw_error = PX4_ISFINITE(desired_yaw) && PX4_ISFINITE(position->heading)
		? wrap_yaw(desired_yaw - position->heading) : NAN;
	report.yaw_rate = _heading_rate;
	report.yaw_control_active = _navigator->get_position_setpoint_triplet()->current.precision_yaw_control;
	_diagnostic_pub.publish(report);
}

bool PrecLand::yaw_usable() const
{
	const uint64_t now = hrt_absolute_time();
	return _target_pose.yaw_valid && PX4_ISFINITE(_target_pose.yaw_abs)
	       && _target_pose.yaw_timestamp > 0 && _target_pose.yaw_timestamp <= now
	       && now - _target_pose.yaw_timestamp <= 500000;
}

float PrecLand::effective_yaw_rate() const
{
	return fmaxf(0.f, fminf(_param_pld_yaw_rate.get(), _param_mpc_yawrauto_max.get())) * YAW_DEG_TO_RAD;
}

// ly_vision_landing: 只在末段门控激活后拥有视觉yaw，按PLD_YAW_RATE与MPC_YAWRAUTO_MAX较小值限速，避开风向标争用。
// ly_vision_landing: 近地承诺锁实际yaw，超时保持当前航向；真正的LAND航向缓存更新与摇杆接管在FlightTaskAuto配套处理。
void PrecLand::update_terminal_yaw()
{
	auto *triplet = _navigator->get_position_setpoint_triplet();
	const auto *position = _navigator->get_local_position();
	const bool enabled = _descent_gate.active()
			     && (_param_pld_yaw_en.get() != 0 || PX4_ISFINITE(_committed_yaw));
	if (!enabled || !position->heading_good_for_control || !PX4_ISFINITE(position->heading)) {
		if (triplet->current.precision_yaw_control) {
			triplet->current.precision_yaw_control = false;
			triplet->current.disable_weather_vane = _saved_weather_vane_disabled;
		}
		return;
	}

	const uint64_t now = hrt_absolute_time();
	if (!PX4_ISFINITE(_yaw_reference)) { _yaw_reference = position->heading; }
	const float dt = _yaw_reference_time > 0 && now >= _yaw_reference_time
			 ? fminf((now - _yaw_reference_time) * 1e-6f, 0.25f) : 0.05f;
	_yaw_reference_time = now;
	if (_descent_gate.committed()) {
		_yaw_reference = _committed_yaw;

	} else if (!_descent_gate.timedOut() && yaw_usable()) {
		const float desired = wrap_yaw(_target_pose.yaw_abs + _param_pld_yaw_off.get() * YAW_DEG_TO_RAD);
		const float error = wrap_yaw(desired - _yaw_reference);
		const float maximum_step = effective_yaw_rate() * dt;
		_yaw_reference = wrap_yaw(_yaw_reference + fmaxf(-maximum_step, fminf(error, maximum_step)));
	}

	triplet->current.yaw = _yaw_reference;
	triplet->current.yaw_valid = true;
	triplet->current.yawspeed_valid = false;
	triplet->current.precision_yaw_control = true;
	triplet->current.disable_weather_vane = true;
}

// ly_vision_landing: 拒绝无效零坐标、非有限值、未来/过期时间戳；有效发布和“本循环新消息”不是同一条件。
bool PrecLand::target_usable(float max_age_s) const
{
	const uint64_t now = hrt_absolute_time();
	return _target_pose_valid && _target_pose.abs_pos_valid && _target_pose.timestamp > 0
	       && _target_pose.timestamp <= now && (now - _target_pose.timestamp) * 1e-6f <= max_age_s;
}

// ly_vision_landing: 末段高度优先用EKF已处理的dist_bottom，其次新鲜视觉z差。
// ly_vision_landing: 不拿Home/气压计相对高度代替真实地面距离，避免高度漂移造成错误进入近地承诺。
float PrecLand::landing_surface_distance(uint8_t &source) const
{
	source = 0;
	const auto *position = _navigator->get_local_position();
	const uint64_t now = hrt_absolute_time();
	if (position->timestamp == 0 || position->timestamp > now || now - position->timestamp > 500000
	    || !position->z_valid || !PX4_ISFINITE(position->z)) { return NAN; }

	if (position->dist_bottom_valid && PX4_ISFINITE(position->dist_bottom) && position->dist_bottom > 0.f) {
		source = 1;
		return position->dist_bottom;
	}

	// Use no Home/barometer height as a substitute for landing-surface distance.
	if (target_usable(fminf(_param_pld_btout.get(), 0.5f))) {
		const float distance = _target_pose.z_abs - position->z;
		if (PX4_ISFINITE(distance) && distance > 0.f) {
			source = 2;
			return distance;
		}
	}

	return NAN;
}

// ly_vision_landing: 跟随EKF本地XY/航向和全局高度重置平移已锁参考；否则缓存仍在旧坐标系会引入人为跳变。
void PrecLand::update_reference_resets()
{
	const auto *position = _navigator->get_local_position();
	if (_heading_reset_counter != position->heading_reset_counter) {
		if (PX4_ISFINITE(position->delta_heading)) {
			_yaw_reference = wrap_yaw(_yaw_reference + position->delta_heading);
			_committed_yaw = wrap_yaw(_committed_yaw + position->delta_heading);
			_previous_heading = wrap_yaw(_previous_heading + position->delta_heading);
		}
		_heading_reset_counter = position->heading_reset_counter;
	}
	if (!position->heading_good_for_control || !PX4_ISFINITE(position->heading)) {
		_heading_rate = NAN;
		_previous_heading_time = 0;

	} else if (position->timestamp != _previous_heading_time) {
		if (_previous_heading_time > 0 && position->timestamp > _previous_heading_time
		    && position->timestamp - _previous_heading_time <= 250000 && PX4_ISFINITE(_previous_heading)) {
			const float dt = (position->timestamp - _previous_heading_time) * 1e-6f;
			const float rate = wrap_yaw(position->heading - _previous_heading) / dt;
			const float alpha = dt / (dt + 0.1f);
			_heading_rate = PX4_ISFINITE(_heading_rate) ? _heading_rate + alpha * (rate - _heading_rate) : rate;

		} else {
			_heading_rate = NAN;
		}
		_previous_heading = position->heading;
		_previous_heading_time = position->timestamp;
	}
	if (_xy_reset_counter != position->xy_reset_counter) {
		const matrix::Vector2f delta(position->delta_xy);
		if (delta.isAllFinite()) {
			_last_valid_target_xy += delta;
			_committed_xy += delta;
			_timeout_hold_xy += delta;
		}
		_xy_reset_counter = position->xy_reset_counter;
	}

	const auto *global = _navigator->get_global_position();
	if (_hold_alt_reset_counter != global->alt_reset_counter) {
		if (_hold_alt_valid && PX4_ISFINITE(global->delta_alt)) { _hold_alt += global->delta_alt; }
		_hold_alt_reset_counter = global->alt_reset_counter;
	}
}

// ly_vision_landing: 暂停时仍追有效靶标XY；超时则锁住实际XY。Z发POSITION+专用暂停位，由下游原生轨迹平滑刹停。
// ly_vision_landing: _hold_alt为进入暂停时的有限导航高度及诊断，不是FlightTaskAuto最终刹停高度，也不是实际接地判据。
void PrecLand::hold_descent(bool target_valid)
{
	auto *triplet = _navigator->get_position_setpoint_triplet();
	const auto *position = _navigator->get_local_position();
	if (!_hold_alt_valid) {
		_hold_alt = _navigator->get_global_position()->alt;
		if (!PX4_ISFINITE(_hold_alt)) { _hold_alt = triplet->current.alt; }
		_hold_alt_valid = PX4_ISFINITE(_hold_alt);
	}

	if (_descent_gate.timedOut() && !_timeout_hold_valid) {
		_timeout_hold_xy = matrix::Vector2f(position->x, position->y);
		_timeout_hold_valid = _timeout_hold_xy.isAllFinite();
	}

	if (target_valid && !_descent_gate.timedOut()) {
		_map_ref.reproject(_target_pose.x_abs, _target_pose.y_abs, triplet->current.lat, triplet->current.lon);

	} else if (_timeout_hold_valid) {
		_map_ref.reproject(_timeout_hold_xy(0), _timeout_hold_xy(1), triplet->current.lat, triplet->current.lon);

	} else if (_last_valid_target_xy.isAllFinite()) {
		_map_ref.reproject(_last_valid_target_xy(0), _last_valid_target_xy(1), triplet->current.lat, triplet->current.lon);
	}

	// Keep a finite navigation altitude, also used by consumers without pause
	// support. FlightTaskAuto brakes Z through its existing trajectory generator
	// and holds the stopping altitude, rather than climbing back to this value.
	triplet->current.alt = _hold_alt;
	triplet->current.type = position_setpoint_s::SETPOINT_TYPE_POSITION;
	triplet->current.precision_descent_pause = true;
	_navigator->set_position_setpoint_triplet_updated();
}

// ly_vision_landing: 将QGC参数与当前有效目标、离地距离、XY速度、yaw输入已有门控；门控不改变RTL返回Home的飞行段。
bool PrecLand::run_terminal_descent_gate()
{
	const auto *position = _navigator->get_local_position();
	const uint64_t now = hrt_absolute_time();
	_gate_height = landing_surface_distance(_height_source);
	const bool target_valid = target_usable(fminf(_param_pld_btout.get(), 0.5f));
	PrecisionLandingGate::Config config{};
	config.enabled = _param_pld_xy_gate.get() != 0;
	config.gate_alt = _param_mpc_land_alt3.get();
	config.commit_alt = _param_pld_xy_cmt_alt.get();
	config.stop_error = _param_pld_xy_stop.get();
	config.resume_error = _param_pld_xy_go.get();
	config.settle_time = _param_pld_xy_t.get();
	config.resume_speed = _param_pld_xy_vel.get();
	config.hold_timeout = _param_pld_xy_tout.get();
	config.yaw_enabled = _param_pld_yaw_en.get() != 0;
	config.yaw_stop = _param_pld_yaw_stop.get() * YAW_DEG_TO_RAD;
	config.yaw_resume = _param_pld_yaw_go.get() * YAW_DEG_TO_RAD;
	config.yaw_settle_rate = _param_pld_yaw_vel.get() * YAW_DEG_TO_RAD;
	config.yaw_rate_limit = effective_yaw_rate();
	PrecisionLandingGate::Input input{};
	input.now = now;
	input.height = _gate_height;
	input.target_valid = target_valid;
	input.horizontal_error = target_valid ? hypotf(_target_pose.x_abs - position->x, _target_pose.y_abs - position->y) : NAN;
	input.horizontal_speed = position->v_xy_valid ? hypotf(position->vx, position->vy) : NAN;
	input.yaw_valid = yaw_usable() && position->heading_good_for_control && PX4_ISFINITE(position->heading);
	input.yaw_error = input.yaw_valid ? wrap_yaw(_target_pose.yaw_abs + _param_pld_yaw_off.get() * YAW_DEG_TO_RAD - position->heading) : NAN;
	input.yaw_rate = _heading_rate;
	input.vehicle_valid = position->timestamp > 0 && position->timestamp <= now && now - position->timestamp <= 500000
			      && position->xy_valid && position->z_valid && position->v_xy_valid
			      && PX4_ISFINITE(position->x) && PX4_ISFINITE(position->y) && PX4_ISFINITE(position->z)
			      && PX4_ISFINITE(position->vx) && PX4_ISFINITE(position->vy)
			      && PX4_ISFINITE(_navigator->get_global_position()->alt)
			      && (!config.yaw_enabled || (position->heading_good_for_control && PX4_ISFINITE(position->heading)));
	const bool was_paused = _descent_gate.paused();
	const bool was_timed_out = _descent_gate.timedOut();
	_descent_gate.update(config, input);

	if (_descent_gate.committed()) {
		// Freeze the endpoint already supplied to PositionSmoothing. Replacing it
		// with measured vehicle XY would make a lagging trajectory accelerate
		// toward that new point, while accepting the current landing error.
		// ly_vision_landing: 2026-10-19修正：承诺后冻结之前下发的落点G，不将G改成实际飞机P，以免平滑器追着P向外加速。
		// ly_vision_landing: 停止更新靶标不等于关闭水平位置/速度环或清积分；内部轨迹R仍平滑跟踪冻结的G。
		const auto &command = _navigator->get_position_setpoint_triplet()->current;
		_committed_xy = matrix::Vector2f(NAN, NAN);

		if (command.valid && PX4_ISFINITE(command.lat) && PX4_ISFINITE(command.lon)) {
			_committed_xy = _map_ref.project(command.lat, command.lon);
		}

		if (!_committed_xy.isAllFinite()) { _committed_xy = _last_valid_target_xy; }

		// Last-resort fallback only when no valid commanded/observed target exists.
		if (!_committed_xy.isAllFinite()) { _committed_xy = matrix::Vector2f(position->x, position->y); }

		if (config.yaw_enabled) { _committed_yaw = position->heading; }
		_hold_alt_valid = false;
		switch_to_state_final_approach();
		run_state_final_approach();
		return true;
	}

	if (!_descent_gate.active()) {
		_hold_alt_valid = false;
		update_terminal_yaw();
		return false;
	}
	if (!was_timed_out && _descent_gate.timedOut()) { _yaw_reference = position->heading; }
	update_terminal_yaw();

	if (_descent_gate.paused()) {
		if (!was_paused) { PX4_INFO("Precland: pause descent for alignment"); }
		if (!was_timed_out && _descent_gate.timedOut()) {
			mavlink_log_critical(_navigator->get_mavlink_log_pub(), "Precland: alignment timeout, take control\t");
		}
		hold_descent(target_valid);
		return true;
	}

	if (was_paused) { PX4_INFO("Precland: aligned, resume descent"); }
	_hold_alt_valid = false;
	// The caller retains LAND and its existing height-dependent descent speed.
	return false;
}

void
PrecLand::updateParams()
{
	ModuleParams::updateParams();

	if (_handle_param_acceleration_hor != PARAM_INVALID) {
		param_get(_handle_param_acceleration_hor, &_param_acceleration_hor);
	}

	if (_handle_param_xy_vel_cruise != PARAM_INVALID) {
		param_get(_handle_param_xy_vel_cruise, &_param_xy_vel_cruise);
	}
}

void
PrecLand::run_state_start()
{
	// check if target visible and go to horizontal approach
	if (switch_to_state_horizontal_approach()) {
		return;
	}

	if (_mode == PrecLandMode::Opportunistic) {
		// could not see the target immediately, so just fall back to normal landing
		switch_to_state_fallback();
	}

	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
	float dist = get_distance_to_next_waypoint(pos_sp_triplet->current.lat, pos_sp_triplet->current.lon,
			_navigator->get_global_position()->lat, _navigator->get_global_position()->lon);

	// check if we've reached the start point
	if (dist < _navigator->get_acceptance_radius()) {
		if (!_point_reached_time) {
			_point_reached_time = hrt_absolute_time();
		}

		// if we don't see the target after 1 second, search for it
		if (_param_pld_srch_tout.get() > 0) {

			if (hrt_absolute_time() - _point_reached_time > 2000000) {
				if (!switch_to_state_search()) {
					switch_to_state_fallback();
				}
			}

		} else {
			switch_to_state_fallback();
		}
	}
}

void
PrecLand::run_state_horizontal_approach()
{
	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
	pos_sp_triplet->current.precision_descent_pause = false;
	const uint64_t now = hrt_absolute_time();
	// ly_vision_landing: 修正高空对准计时：新鲜缓存可连续证明对准；误差超限或循环中断超0.25秒必须清零2秒计时。
	// A scheduling gap is not evidence that alignment remained satisfactory.
	if (_horizontal_alignment_last_check > 0
	    && (now < _horizontal_alignment_last_check || now - _horizontal_alignment_last_check > 250000)) {
		_point_reached_time = 0;
	}
	_horizontal_alignment_last_check = now;

	// check if target visible, if not go to start
	if (!check_state_conditions(PrecLandState::HorizontalApproach)) {
		PX4_WARN("%s, state: %i", LOST_TARGET_ERROR_MESSAGE, (int) _state);

		// Stay at current position for searching for the landing target
		pos_sp_triplet->current.lat = _navigator->get_global_position()->lat;
		pos_sp_triplet->current.lon = _navigator->get_global_position()->lon;
		pos_sp_triplet->current.alt = _navigator->get_global_position()->alt;

		if (!switch_to_state_start()) {
			switch_to_state_fallback();
		}

		return;
	}

	if (check_state_conditions(PrecLandState::DescendAboveTarget)) {
		if (!_point_reached_time) {
			_point_reached_time = now;
		}

		if (now - _point_reached_time > 2000000) {
			// if close enough for descent above target go to descend above target
			if (switch_to_state_descend_above_target()) {

				return;
			}
		}

	} else {
		_point_reached_time = 0; // Require continuous alignment, not accumulated visits.
	}

	// Invalid estimator publications contain zero coordinates. Never pursue those.
	float x = _last_valid_target_xy(0);
	float y = _last_valid_target_xy(1);

	slewrate(x, y);

	// XXX need to transform to GPS coords because mc_pos_control only looks at that
	_map_ref.reproject(x, y, pos_sp_triplet->current.lat, pos_sp_triplet->current.lon);

	pos_sp_triplet->current.alt = _approach_alt;
	pos_sp_triplet->current.type = position_setpoint_s::SETPOINT_TYPE_POSITION;

	_navigator->set_position_setpoint_triplet_updated();
}

void
PrecLand::run_state_descend_above_target()
{
	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
	pos_sp_triplet->current.precision_descent_pause = false;
	if (run_terminal_descent_gate()) { return; }

	// check if target visible
	if (!check_state_conditions(PrecLandState::DescendAboveTarget)) {
		if (!switch_to_state_final_approach()) {
			PX4_WARN("%s, state: %i", LOST_TARGET_ERROR_MESSAGE, (int) _state);

			// Stay at current position for searching for the target
			pos_sp_triplet->current.lat = _navigator->get_global_position()->lat;
			pos_sp_triplet->current.lon = _navigator->get_global_position()->lon;
			pos_sp_triplet->current.alt = _navigator->get_global_position()->alt;

			if (!switch_to_state_start()) {
				switch_to_state_fallback();
			}
		}

		return;
	}

	// XXX need to transform to GPS coords because mc_pos_control only looks at that
	_map_ref.reproject(_target_pose.x_abs, _target_pose.y_abs, pos_sp_triplet->current.lat, pos_sp_triplet->current.lon);

	pos_sp_triplet->current.type = position_setpoint_s::SETPOINT_TYPE_LAND;

	_navigator->set_position_setpoint_triplet_updated();
}

void
PrecLand::run_state_final_approach()
{
	if (_descent_gate.committed()) {
		// ly_vision_landing: 承诺段不再更新视觉目标或重新因XY误差暂停；保持旧落点执行LAND，接地/上锁仍由原链路决定。
		update_terminal_yaw();
		// Hold the existing landing endpoint. Do not chase new marker estimates
		// or rebase the trajectory onto measured vehicle position at commitment.
		auto *triplet = _navigator->get_position_setpoint_triplet();
		_map_ref.reproject(_committed_xy(0), _committed_xy(1), triplet->current.lat, triplet->current.lon);
		triplet->current.type = position_setpoint_s::SETPOINT_TYPE_LAND;
		triplet->current.precision_descent_pause = false;
		_navigator->set_position_setpoint_triplet_updated();
	}
}

void
PrecLand::run_state_search()
{
	// check if we can see the target
	if (check_state_conditions(PrecLandState::HorizontalApproach)) {
		if (!_target_acquired_time) {
			// target just became visible. Stop climbing, but give it some margin so we don't stop too apruptly
			_target_acquired_time = hrt_absolute_time();
			position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
			float new_alt = _navigator->get_global_position()->alt + 1.0f;
			pos_sp_triplet->current.alt = new_alt < pos_sp_triplet->current.alt ? new_alt : pos_sp_triplet->current.alt;
			_navigator->set_position_setpoint_triplet_updated();
		}

	}

	// stay at that height for a second to allow the vehicle to settle
	if (_target_acquired_time && (hrt_absolute_time() - _target_acquired_time) > 1000000) {
		// try to switch to horizontal approach
		if (switch_to_state_horizontal_approach()) {
			return;
		}
	}

	// check if search timed out and go to fallback
	if (hrt_absolute_time() - _state_start_time > _param_pld_srch_tout.get()*SEC2USEC) {
		PX4_WARN("Search timed out");

		switch_to_state_fallback();
	}
}

void
PrecLand::run_state_fallback()
{
	// nothing to do, will land
}

bool
PrecLand::switch_to_state_start()
{
	if (check_state_conditions(PrecLandState::Start)) {
		position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
		pos_sp_triplet->current.type = position_setpoint_s::SETPOINT_TYPE_POSITION;
		_navigator->set_position_setpoint_triplet_updated();
		_search_cnt++;

		_point_reached_time = 0;

		_state = PrecLandState::Start;
		_state_start_time = hrt_absolute_time();
		return true;
	}

	return false;
}

bool
PrecLand::switch_to_state_horizontal_approach()
{
	if (check_state_conditions(PrecLandState::HorizontalApproach)) {
		print_state_switch_message("horizontal approach");
		_approach_alt = _navigator->get_global_position()->alt;

		_point_reached_time = 0;
		_horizontal_alignment_last_check = 0;

		_state = PrecLandState::HorizontalApproach;
		_state_start_time = hrt_absolute_time();
		return true;
	}

	return false;
}

bool
PrecLand::switch_to_state_descend_above_target()
{
	if (check_state_conditions(PrecLandState::DescendAboveTarget)) {
		print_state_switch_message("descend");
		_state = PrecLandState::DescendAboveTarget;
		_state_start_time = hrt_absolute_time();
		return true;
	}

	return false;
}

bool
PrecLand::switch_to_state_final_approach()
{
	if (check_state_conditions(PrecLandState::FinalApproach)) {
		print_state_switch_message("final approach");
		_state = PrecLandState::FinalApproach;
		_state_start_time = hrt_absolute_time();
		return true;
	}

	return false;
}

bool
PrecLand::switch_to_state_search()
{
	PX4_INFO("Climbing to search altitude.");
	vehicle_local_position_s *vehicle_local_position = _navigator->get_local_position();

	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
	pos_sp_triplet->current.alt = vehicle_local_position->ref_alt + _param_pld_srch_alt.get();
	pos_sp_triplet->current.type = position_setpoint_s::SETPOINT_TYPE_POSITION;
	_navigator->set_position_setpoint_triplet_updated();

	_target_acquired_time = 0;

	_state = PrecLandState::Search;
	_state_start_time = hrt_absolute_time();
	return true;
}

bool
PrecLand::switch_to_state_fallback()
{
	print_state_switch_message("fallback");
	position_setpoint_triplet_s *pos_sp_triplet = _navigator->get_position_setpoint_triplet();
	pos_sp_triplet->current.lat = _navigator->get_global_position()->lat;
	pos_sp_triplet->current.lon = _navigator->get_global_position()->lon;
	pos_sp_triplet->current.alt = _navigator->get_global_position()->alt;
	pos_sp_triplet->current.type = position_setpoint_s::SETPOINT_TYPE_LAND;
	pos_sp_triplet->current.precision_descent_pause = false;
	_navigator->set_position_setpoint_triplet_updated();

	_state = PrecLandState::Fallback;
	_state_start_time = hrt_absolute_time();
	return true;
}

bool
PrecLand::switch_to_state_done()
{
	auto *triplet = _navigator->get_position_setpoint_triplet();
	if (triplet->current.precision_descent_pause) {
		triplet->current.precision_descent_pause = false;
		_navigator->set_position_setpoint_triplet_updated();
	}
	_state = PrecLandState::Done;
	_state_start_time = hrt_absolute_time();
	return true;
}

void PrecLand::print_state_switch_message(const char *state_name)
{
	PX4_INFO("Precland: switching to %s", state_name);
}

bool PrecLand::check_state_conditions(PrecLandState state)
{
	vehicle_local_position_s *vehicle_local_position = _navigator->get_local_position();

	switch (state) {
	case PrecLandState::Start:
		return _search_cnt <= _param_pld_max_srch.get();

	case PrecLandState::HorizontalApproach:

		// if we're already in this state, only want to make it invalid if we reached the target but can't see it anymore
		if (_state == PrecLandState::HorizontalApproach) {
			if (!_last_valid_target_xy.isAllFinite()
			    || (fabsf(_last_valid_target_xy(0) - vehicle_local_position->x) < _param_pld_hacc_rad.get()
				&& fabsf(_last_valid_target_xy(1) - vehicle_local_position->y) < _param_pld_hacc_rad.get())) {
				// we've reached the position where we last saw the target. If we don't see it now, we need to do something
				return target_usable(_param_pld_btout.get());

			} else {
				// We've seen the target sometime during horizontal approach.
				// Even if we don't see it as we're moving towards it, continue approaching last known location
				return true;
			}
		}

		// If we're trying to switch to this state, the target needs to be visible
		return _target_pose_updated && target_usable(_param_pld_btout.get());

	case PrecLandState::DescendAboveTarget:

		// if we're already in this state, only leave it if target becomes unusable, don't care about horizontall offset to target
		if (_state == PrecLandState::DescendAboveTarget) {
			// if we're close to the ground, we're more critical of target timeouts so we quickly go into descend
			if (check_state_conditions(PrecLandState::FinalApproach)) {
				return hrt_absolute_time() - _target_pose.timestamp < 500000; // 0.5s

			} else {
				return _target_pose_valid && _target_pose.abs_pos_valid;
			}

		} else {
			// ly_vision_landing: 进入下降的判据不能要求每个导航循环都有新消息，否则估计器/导航异步会反复打断对准计时。
			// ly_vision_landing: 仍要求目标和飞机位置均新鲜有效，不能把旧数据当作连续稳定证据。
			// Use a fresh cached target between asynchronous estimator publications.
			// "No new message this loop" is not "alignment lost". Do not count
			// old target/vehicle data as continuous evidence of alignment either.
			const uint64_t now = hrt_absolute_time();
			return target_usable(fminf(_param_pld_btout.get(), 0.5f))
			       && vehicle_local_position->timestamp > 0 && vehicle_local_position->timestamp <= now
			       && now - vehicle_local_position->timestamp <= 500000
			       && vehicle_local_position->xy_valid
			       && PX4_ISFINITE(vehicle_local_position->x) && PX4_ISFINITE(vehicle_local_position->y)
			       && fabsf(_target_pose.x_abs - vehicle_local_position->x) < _param_pld_hacc_rad.get()
			       && fabsf(_target_pose.y_abs - vehicle_local_position->y) < _param_pld_hacc_rad.get();
		}

	case PrecLandState::FinalApproach:
		if ((_param_pld_xy_gate.get() != 0 && _param_mpc_land_alt3.get() > 0.f) || _descent_gate.active()) {
			return _descent_gate.committed();
		}
		return _target_pose_valid && _target_pose.abs_pos_valid
		       && (_target_pose.z_abs - vehicle_local_position->z) < _param_pld_fappr_alt.get();

	case PrecLandState::Search:
		return true;

	case PrecLandState::Fallback:
		return true;

	default:
		return false;
	}
}

void PrecLand::slewrate(float &sp_x, float &sp_y)
{
	matrix::Vector2f sp_curr(sp_x, sp_y);
	uint64_t now = hrt_absolute_time();

	float dt = (now - _last_slewrate_time);

	if (dt < 1) {
		// bad dt, can't divide by it
		return;
	}

	dt /= SEC2USEC;

	if (!_last_slewrate_time) {
		// running the first time since switching to precland

		// assume dt will be about 50000us
		dt = 50000 / SEC2USEC;

		// set a best guess for previous setpoints for smooth transition
		_sp_pev = _map_ref.project(_navigator->get_position_setpoint_triplet()->current.lat,
					   _navigator->get_position_setpoint_triplet()->current.lon);
		_sp_pev_prev(0) = _sp_pev(0) - _navigator->get_local_position()->vx * dt;
		_sp_pev_prev(1) = _sp_pev(1) - _navigator->get_local_position()->vy * dt;
	}

	_last_slewrate_time = now;

	// limit the setpoint speed to the maximum cruise speed
	matrix::Vector2f sp_vel = (sp_curr - _sp_pev) / dt; // velocity of the setpoints

	if (sp_vel.length() > _param_xy_vel_cruise) {
		sp_vel = sp_vel.normalized() * _param_xy_vel_cruise;
		sp_curr = _sp_pev + sp_vel * dt;
	}

	// limit the setpoint acceleration to the maximum acceleration
	matrix::Vector2f sp_acc = (sp_curr - _sp_pev * 2 + _sp_pev_prev) / (dt * dt); // acceleration of the setpoints

	if (sp_acc.length() > _param_acceleration_hor) {
		sp_acc = sp_acc.normalized() * _param_acceleration_hor;
		sp_curr = _sp_pev * 2 - _sp_pev_prev + sp_acc * (dt * dt);
	}

	// limit the setpoint speed such that we can stop at the setpoint given the maximum acceleration/deceleration
	float max_spd = sqrtf(_param_acceleration_hor * ((matrix::Vector2f)(_sp_pev - matrix::Vector2f(sp_x,
			      sp_y))).length());
	sp_vel = (sp_curr - _sp_pev) / dt; // velocity of the setpoints

	if (sp_vel.length() > max_spd) {
		sp_vel = sp_vel.normalized() * max_spd;
		sp_curr = _sp_pev + sp_vel * dt;
	}

	_sp_pev_prev = _sp_pev;
	_sp_pev = sp_curr;

	sp_x = sp_curr(0);
	sp_y = sp_curr(1);
}
