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

#pragma once

#include <matrix/math.hpp>

namespace landing_target_estimator
{

class StaticTargetKalmanFilter
{
public:
	void reset();
	void init(const matrix::Vector3f &measurement, float initial_variance);
	void predict(float dt, float process_variance_rate);
	bool update(const matrix::Vector3f &measurement, const matrix::Vector3f &measurement_variance,
		    float innovation_gate);
	void shiftState(const matrix::Vector3f &delta);

	bool initialized() const { return _initialized; }
	const matrix::Vector3f &state() const { return _state; }
	const matrix::Vector3f &variance() const { return _variance; }
	const matrix::Vector3f &innovation() const { return _innovation; }
	const matrix::Vector3f &innovationVariance() const { return _innovation_variance; }

private:
	bool _initialized{false};
	matrix::Vector3f _state{};
	matrix::Vector3f _variance{};
	matrix::Vector3f _innovation{};
	matrix::Vector3f _innovation_variance{};
};

} // namespace landing_target_estimator
