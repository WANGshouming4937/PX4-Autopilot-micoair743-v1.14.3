/****************************************************************************
 *
 *   Copyright (c) 2026 PX4 Development Team. All rights reserved.
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

#include "StaticTargetKalmanFilter.h"

#include <math.h>

namespace landing_target_estimator
{

void StaticTargetKalmanFilter::reset()
{
	_initialized = false;
	_state.zero();
	_variance.zero();
	_innovation.zero();
	_innovation_variance.zero();
}

void StaticTargetKalmanFilter::init(const matrix::Vector3f &measurement, float initial_variance)
{
	_state = measurement;
	_variance = matrix::Vector3f{initial_variance, initial_variance, initial_variance};
	_innovation.zero();
	_innovation_variance.zero();
	_initialized = true;
}

void StaticTargetKalmanFilter::predict(float dt, float process_variance_rate)
{
	if (!_initialized || dt <= 0.f) {
		return;
	}

	const float variance_increment = fmaxf(process_variance_rate, 0.f) * dt;

	for (int axis = 0; axis < 3; ++axis) {
		_variance(axis) += variance_increment;
	}
}

bool StaticTargetKalmanFilter::update(const matrix::Vector3f &measurement,
		const matrix::Vector3f &measurement_variance, float innovation_gate)
{
	if (!_initialized) {
		return false;
	}

	for (int axis = 0; axis < 3; ++axis) {
		const float measurement_var = fmaxf(measurement_variance(axis), 1e-6f);
		_innovation(axis) = measurement(axis) - _state(axis);
		_innovation_variance(axis) = _variance(axis) + measurement_var;

		const float normalized_innovation_squared = _innovation(axis) * _innovation(axis)
				/ _innovation_variance(axis);

		if (innovation_gate > 0.f && normalized_innovation_squared > innovation_gate) {
			return false;
		}
	}

	for (int axis = 0; axis < 3; ++axis) {
		const float kalman_gain = _variance(axis) / _innovation_variance(axis);
		_state(axis) += kalman_gain * _innovation(axis);
		_variance(axis) = fmaxf((1.f - kalman_gain) * _variance(axis), 1e-6f);
	}

	return true;
}

void StaticTargetKalmanFilter::shiftState(const matrix::Vector3f &delta)
{
	if (_initialized) {
		_state += delta;
	}
}

} // namespace landing_target_estimator
