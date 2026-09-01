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

#pragma once

#include <drivers/drv_hrt.h>
#include <parameters/param.h>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/topics/landing_target_innovations.h>
#include <uORB/topics/landing_target_observation.h>
#include <uORB/topics/landing_target_pose.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_local_position.h>
#include <matrix/math.hpp>

#include "StaticTargetKalmanFilter.h"

using namespace time_literals;

namespace landing_target_estimator
{

class LandingTargetEstimator
{
public:
	LandingTargetEstimator();
	virtual ~LandingTargetEstimator() = default;

	void update();

private:
	struct ParameterHandles {
		param_t position_uncertainty_initial;
		param_t measurement_uncertainty_xy;
		param_t measurement_uncertainty_z;
		param_t process_uncertainty;
		param_t innovation_gate;
		param_t timeout;
	} _param_handles{};

	struct Parameters {
		float position_uncertainty_initial{0.1f};
		float measurement_uncertainty_xy{0.04f};
		float measurement_uncertainty_z{0.09f};
		float process_uncertainty{0.0025f};
		float innovation_gate{9.f};
		float timeout{0.8f};
	} _params{};

	uORB::Publication<landing_target_pose_s> _target_pose_pub{ORB_ID(landing_target_pose)};
	uORB::Publication<landing_target_innovations_s> _target_innovations_pub{ORB_ID(landing_target_innovations)};

	uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};
	uORB::Subscription _landing_target_observation_sub{ORB_ID(landing_target_observation)};
	uORB::Subscription _vehicle_attitude_sub{ORB_ID(vehicle_attitude)};
	uORB::Subscription _vehicle_local_position_sub{ORB_ID(vehicle_local_position)};

	vehicle_attitude_s _vehicle_attitude{};
	vehicle_local_position_s _vehicle_local_position{};

	StaticTargetKalmanFilter _filter{};
	hrt_abstime _last_predict{0};
	hrt_abstime _last_valid_observation{0};
	bool _measurement_rejected{false};

	bool _reset_counters_initialized{false};
	uint8_t _xy_reset_counter{0};
	uint8_t _z_reset_counter{0};

	void _check_params(bool force);
	void _update_params();
	void _update_vehicle_state();
	void _handle_observation(const landing_target_observation_s &observation, hrt_abstime now);
	void _publish_target_pose(hrt_abstime now);
	bool _vehicle_state_valid() const;
};

} // namespace landing_target_estimator
