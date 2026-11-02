/****************************************************************************
 *
 *   Copyright (c) 2013-2026 PX4 Development Team. All rights reserved.
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

#include "LandingTargetEstimator.h"

#include <px4_platform_common/defines.h>
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>

#include <math.h>

namespace landing_target_estimator
{

LandingTargetEstimator::LandingTargetEstimator()
{
	_param_handles.position_uncertainty_initial = param_find("LTEST_POS_UNC_IN");
	_param_handles.measurement_uncertainty_xy = param_find("LTEST_MEAS_XY");
	_param_handles.measurement_uncertainty_z = param_find("LTEST_MEAS_Z");
	_param_handles.process_uncertainty = param_find("LTEST_PROC_UNC");
	_param_handles.innovation_gate = param_find("LTEST_GATE");
	_param_handles.timeout = param_find("LTEST_TIMEOUT");
	_check_params(true);
}

void LandingTargetEstimator::update()
{
	_check_params(false);
	_update_vehicle_state();

	const hrt_abstime now = hrt_absolute_time();

	if (_filter.initialized()) {
		const float dt = math::constrain((now - _last_predict) * 1e-6f, 0.f, 1.f);
		_filter.predict(dt, _params.process_uncertainty);
		_last_predict = now;
	}

	landing_target_observation_s observation{};

	if (_landing_target_observation_sub.update(&observation)) {
		_handle_observation(observation, now);
	}

	if (_filter.initialized()) {
		const hrt_abstime timeout_us = static_cast<hrt_abstime>(_params.timeout * 1e6f);

		if (now - _last_valid_observation > timeout_us) {
			PX4_INFO("landing target observation timed out");
			auto report = _debug_snapshot(now);
			report.result = 6; // timeout: no associated fresh measurement
			_filter.reset();
			_publish_debug(report);
			_measurement_rejected = false;
		}
	}

	_publish_target_pose(now);
}

void LandingTargetEstimator::_update_vehicle_state()
{
	_vehicle_attitude_sub.update(&_vehicle_attitude);
	const uint64_t attitude_time = _vehicle_attitude.timestamp_sample > 0
				       ? _vehicle_attitude.timestamp_sample : _vehicle_attitude.timestamp;
	_heading.pushAttitude(attitude_time, _vehicle_attitude.q, _vehicle_attitude.quat_reset_counter, hrt_absolute_time());

	if (_vehicle_local_position_sub.update(&_vehicle_local_position)) {
		if (_reset_counters_initialized && _filter.initialized()) {
			matrix::Vector3f reset_delta{};
			bool position_reset = false;

			if (_vehicle_local_position.xy_reset_counter != _xy_reset_counter
			    && PX4_ISFINITE(_vehicle_local_position.delta_xy[0])
			    && PX4_ISFINITE(_vehicle_local_position.delta_xy[1])) {
				reset_delta(0) = _vehicle_local_position.delta_xy[0];
				reset_delta(1) = _vehicle_local_position.delta_xy[1];
				position_reset = true;
			}

			if (_vehicle_local_position.z_reset_counter != _z_reset_counter
			    && PX4_ISFINITE(_vehicle_local_position.delta_z)) {
				reset_delta(2) = _vehicle_local_position.delta_z;
				position_reset = true;
			}

			if (position_reset) {
				_filter.shiftState(reset_delta);
			}
		}

		_xy_reset_counter = _vehicle_local_position.xy_reset_counter;
		_z_reset_counter = _vehicle_local_position.z_reset_counter;
		_reset_counters_initialized = true;
	}
}

void LandingTargetEstimator::_handle_observation(const landing_target_observation_s &observation, hrt_abstime now)
{
	auto report = _debug_snapshot(now);
	report.observation_timestamp = observation.timestamp;
	report.observation_timestamp_sample = observation.timestamp_sample;
	report.source_timestamp = observation.source_timestamp;
	report.source_system = observation.source_system;
	report.source_component = observation.source_component;
	for (int i = 0; i < 3; ++i) { report.observation_body[i] = observation.position_body[i]; }
	if (!observation.position_valid || observation.frame != landing_target_observation_s::FRAME_BODY_FRD
	    || !_vehicle_state_valid()) {
		report.result = !observation.position_valid || observation.frame != landing_target_observation_s::FRAME_BODY_FRD ? 0 : 1;
		_publish_debug(report);
		return;
	}

	if (!PX4_ISFINITE(observation.position_body[0])
	    || !PX4_ISFINITE(observation.position_body[1])
	    || !PX4_ISFINITE(observation.position_body[2])) {
		report.result = 2;
		_publish_debug(report);
		return;
	}

	const matrix::Vector3f target_body{observation.position_body[0],
					  observation.position_body[1],
					  observation.position_body[2]};
	// ly_vision_landing: 方向另用历史姿态匹配观测时间；这一时间补偿只用于航向，下面 XYZ 仍使用当前缓存姿态与位置。
	if (observation.orientation_valid) {
		const uint64_t sample_time = observation.timestamp_sample > 0 ? observation.timestamp_sample : observation.timestamp;
		report.heading_result = static_cast<uint8_t>(_heading.observe(observation.q_body_target, sample_time, now));
	}
	const matrix::Vector3f vehicle_position{_vehicle_local_position.x,
					       _vehicle_local_position.y,
					       _vehicle_local_position.z};
	const matrix::Quaternion<float> vehicle_attitude(&_vehicle_attitude.q[0]);
	const matrix::Dcmf rotation_body_to_ned(vehicle_attitude);
	const matrix::Vector3f target_absolute_measurement = vehicle_position + rotation_body_to_ned * target_body;

	for (int axis = 0; axis < 3; ++axis) { report.measurement_ned[axis] = target_absolute_measurement(axis); }
	bool measurement_accepted = true;

	if (!_filter.initialized()) {
		report.result = 3;
		_filter.init(target_absolute_measurement, fmaxf(_params.position_uncertainty_initial, 1e-6f));
		_last_predict = now;

	} else {
		const matrix::Vector3f measurement_variance{_params.measurement_uncertainty_xy,
							     _params.measurement_uncertainty_xy,
							     _params.measurement_uncertainty_z};
		// Calculate all diagnostic innovations before update(): the original gate may return on the first bad axis.
		for (int axis = 0; axis < 3; ++axis) {
			report.innovation[axis] = target_absolute_measurement(axis) - _filter.state()(axis);
			report.innovation_variance[axis] = _filter.variance()(axis) + fmaxf(measurement_variance(axis), 1e-6f);
		}
		measurement_accepted = _filter.update(target_absolute_measurement, measurement_variance,
						      _params.innovation_gate);
		report.result = measurement_accepted ? 4 : 5;
	}

	if (measurement_accepted) {
		_last_valid_observation = observation.timestamp > 0 && observation.timestamp <= now
					  ? observation.timestamp
					  : now;

		if (_measurement_rejected) {
			PX4_INFO("landing target measurement accepted again");
		}

		_measurement_rejected = false;

	} else {
		if (!_measurement_rejected) {
			PX4_INFO("landing target measurement rejected");
		}

		_measurement_rejected = true;
	}

	landing_target_innovations_s innovations{};
	innovations.timestamp = observation.timestamp > 0 ? observation.timestamp : now;
	innovations.innov_x = _filter.innovation()(0);
	innovations.innov_y = _filter.innovation()(1);
	innovations.innov_cov_x = _filter.innovationVariance()(0);
	innovations.innov_cov_y = _filter.innovationVariance()(1);
	_target_innovations_pub.publish(innovations);
	_publish_debug(report);
}

// 精简诊断：记录位置创新、处理结果和航向观测结果，不参与控制。
landing_target_estimator_debug_s LandingTargetEstimator::_debug_snapshot(hrt_abstime now) const
{
	landing_target_estimator_debug_s report{};
	report.timestamp = now;
	for (int i = 0; i < 3; ++i) {
		report.measurement_ned[i] = NAN;
		report.innovation[i] = NAN; report.innovation_variance[i] = NAN;
	}
	return report;
}

void LandingTargetEstimator::_publish_debug(landing_target_estimator_debug_s &report)
{
	for (int i = 0; i < 3; ++i) {
		report.variance_after[i] = _filter.initialized() ? _filter.variance()(i) : NAN;
	}
	report.yaw_raw = _heading.rawYaw();
	report.yaw_attitude_time_error_s = _heading.attitudeTimeError();
	_debug_pub.publish(report);
}

void LandingTargetEstimator::_publish_target_pose(hrt_abstime now)
{
	landing_target_pose_s target_pose{};
	target_pose.timestamp = _last_valid_observation;
	target_pose.is_static = true;
	target_pose.yaw_valid = _heading.valid(now);
	target_pose.yaw_abs = _heading.yaw();
	target_pose.yaw_timestamp = _heading.timestamp();

	const hrt_abstime timeout_us = static_cast<hrt_abstime>(_params.timeout * 1e6f);
	const bool observation_fresh = _filter.initialized()
				       && _last_valid_observation > 0
				       && now - _last_valid_observation <= timeout_us;

	if (observation_fresh && _vehicle_state_valid()) {
		const matrix::Vector3f &target_absolute = _filter.state();
		const matrix::Vector3f target_relative{target_absolute(0) - _vehicle_local_position.x,
						       target_absolute(1) - _vehicle_local_position.y,
						       target_absolute(2) - _vehicle_local_position.z};

		target_pose.abs_pos_valid = true;
		target_pose.x_abs = target_absolute(0);
		target_pose.y_abs = target_absolute(1);
		target_pose.z_abs = target_absolute(2);

		target_pose.rel_pos_valid = true;
		target_pose.x_rel = target_relative(0);
		target_pose.y_rel = target_relative(1);
		target_pose.z_rel = target_relative(2);
		target_pose.cov_x_rel = _filter.variance()(0);
		target_pose.cov_y_rel = _filter.variance()(1);

		if (_vehicle_local_position.v_xy_valid) {
			target_pose.rel_vel_valid = false;
			target_pose.vx_rel = -_vehicle_local_position.vx;
			target_pose.vy_rel = -_vehicle_local_position.vy;
		}
	}

	_target_pose_pub.publish(target_pose);
}

bool LandingTargetEstimator::_vehicle_state_valid() const
{
	return _vehicle_attitude.timestamp > 0
	       && _vehicle_local_position.timestamp > 0
	       && _vehicle_local_position.xy_valid
	       && _vehicle_local_position.z_valid
	       && PX4_ISFINITE(_vehicle_local_position.x)
	       && PX4_ISFINITE(_vehicle_local_position.y)
	       && PX4_ISFINITE(_vehicle_local_position.z)
	       && PX4_ISFINITE(_vehicle_attitude.q[0])
	       && PX4_ISFINITE(_vehicle_attitude.q[1])
	       && PX4_ISFINITE(_vehicle_attitude.q[2])
	       && PX4_ISFINITE(_vehicle_attitude.q[3]);
}

void LandingTargetEstimator::_check_params(bool force)
{
	if (_parameter_update_sub.updated() || force) {
		parameter_update_s parameter_update{};
		_parameter_update_sub.copy(&parameter_update);
		_update_params();
	}
}

void LandingTargetEstimator::_update_params()
{
	param_get(_param_handles.position_uncertainty_initial, &_params.position_uncertainty_initial);
	param_get(_param_handles.measurement_uncertainty_xy, &_params.measurement_uncertainty_xy);
	param_get(_param_handles.measurement_uncertainty_z, &_params.measurement_uncertainty_z);
	param_get(_param_handles.process_uncertainty, &_params.process_uncertainty);
	param_get(_param_handles.innovation_gate, &_params.innovation_gate);
	param_get(_param_handles.timeout, &_params.timeout);

	_params.position_uncertainty_initial = fmaxf(_params.position_uncertainty_initial, 1e-6f);
	_params.measurement_uncertainty_xy = fmaxf(_params.measurement_uncertainty_xy, 1e-6f);
	_params.measurement_uncertainty_z = fmaxf(_params.measurement_uncertainty_z, 1e-6f);
	_params.process_uncertainty = fmaxf(_params.process_uncertainty, 0.f);
	_params.innovation_gate = fmaxf(_params.innovation_gate, 0.f);
	_params.timeout = math::constrain(_params.timeout, 0.1f, 10.f);
}

} // namespace landing_target_estimator
