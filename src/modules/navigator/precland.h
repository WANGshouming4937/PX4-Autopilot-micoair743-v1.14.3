/***************************************************************************
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
 * @file precland.h
 *
 * Helper class to do precision landing with a landing target
 *
 * @author Nicolas de Palezieux (Sunflower Labs) <ndepal@gmail.com>
 */

#pragma once

#include <matrix/math.hpp>
#include <lib/geo/geo.h>
#include <px4_platform_common/module_params.h>
#include <uORB/Subscription.hpp>
#include <uORB/topics/landing_target_pose.h>
#include <uORB/Publication.hpp>
#include <uORB/topics/precision_landing_status.h>

#include "navigator_mode.h"
#include "mission_block.h"
#include "PrecisionLandingGate.hpp"

enum class PrecLandState {
	Start, // Starting state
	HorizontalApproach, // Positioning over landing target while maintaining altitude
	DescendAboveTarget, // Stay over landing target while descending
	FinalApproach, // Final landing approach, even without landing target
	Search, // Search for landing target
	Fallback, // Fallback landing method
	Done // Done landing
};

enum class PrecLandMode {
	Opportunistic = 1, // only do precision landing if landing target visible at the beginning
	Required = 2 // try to find landing target if not visible at the beginning
};

class PrecLand : public MissionBlock, public ModuleParams
{
public:
	PrecLand(Navigator *navigator);
	~PrecLand() override = default;

	void on_activation() override;
	void on_active() override;
	void on_inactivation() override;

	void set_mode(PrecLandMode mode) { _mode = mode; };

	PrecLandMode get_mode() { return _mode; };

	bool is_activated() { return _is_activated; };

private:

	void updateParams() override;

	// run the control loop for each state
	void run_state_start();
	void run_state_horizontal_approach();
	void run_state_descend_above_target();
	void run_state_final_approach();
	void run_state_search();
	void run_state_fallback();

	// attempt to switch to a different state. Returns true if state change was successful, false otherwise
	bool switch_to_state_start();
	bool switch_to_state_horizontal_approach();
	bool switch_to_state_descend_above_target();
	bool switch_to_state_final_approach();
	bool switch_to_state_search();
	bool switch_to_state_fallback();
	bool switch_to_state_done();

	void print_state_switch_message(const char *state_name);
	// ly_vision_landing: 以下为本项目诊断、目标有效性、末段XY/yaw门控接口；原生PrecLand阶段调度仍是入口。
	void publish_diagnostic_status(bool force = false);
	bool target_usable(float max_age_s) const;
	float landing_surface_distance(uint8_t &source) const;
	bool run_terminal_descent_gate();
	void hold_descent(bool target_valid);
	void update_reference_resets();
	bool yaw_usable() const;
	float effective_yaw_rate() const;
	void update_terminal_yaw();
	uORB::Publication<precision_landing_status_s> _diagnostic_pub{ORB_ID(precision_landing_status)};
	uint64_t _last_diagnostic_time{0};
	PrecLandState _last_diagnostic_state{PrecLandState::Start};
	uint8_t _last_gate_reason{0};

	// check if a given state could be changed into. Return true if possible to transition to state, false otherwise
	bool check_state_conditions(PrecLandState state);
	void slewrate(float &sp_x, float &sp_y);

	landing_target_pose_s _target_pose{}; /**< precision landing target position */

	uORB::Subscription _target_pose_sub{ORB_ID(landing_target_pose)};
	bool _target_pose_valid{false}; /**< whether we have received a landing target position message */
	bool _target_pose_updated{false}; /**< wether the landing target position message is updated */
	// ly_vision_landing: 三种缓存各司其职：最近有效靶标、承诺时旧命令落点、超时后实际悬停点，不能相互当作测量真值。
	matrix::Vector2f _last_valid_target_xy{NAN, NAN};
	matrix::Vector2f _committed_xy{NAN, NAN};
	matrix::Vector2f _timeout_hold_xy{NAN, NAN};
	PrecisionLandingGate _descent_gate;
	float _hold_alt{NAN};
	float _gate_height{NAN};
	uint8_t _height_source{0}; // 0 unavailable, 1 local-position range, 2 fresh visual target
	uint8_t _hold_alt_reset_counter{0};
	uint8_t _xy_reset_counter{0};
	bool _hold_alt_valid{false};
	bool _timeout_hold_valid{false};
	float _yaw_reference{NAN};
	float _committed_yaw{NAN};
	float _heading_rate{NAN};
	float _previous_heading{NAN};
	uint64_t _yaw_reference_time{0};
	uint64_t _previous_heading_time{0};
	uint8_t _heading_reset_counter{0};
	bool _saved_weather_vane_disabled{false};

	MapProjection _map_ref{}; /**< class for local/global projections */

	uint64_t _state_start_time{0}; /**< time when we entered current state */
	uint64_t _last_slewrate_time{0}; /**< time when we last limited setpoint changes */
	uint64_t _target_acquired_time{0}; /**< time when we first saw the landing target during search */
	uint64_t _point_reached_time{0}; /**< time when we reached a setpoint */
	// ly_vision_landing: 记录导航检查时刻而非新观测时刻，检测调度中断；配合新鲜缓存判据维持连续2秒对准计时。
	uint64_t _horizontal_alignment_last_check{0}; /**< reject unobserved gaps in alignment timing */

	int _search_cnt{0}; /**< counter of how many times we had to search for the landing target */
	float _approach_alt{0.0f}; /**< altitude at which to stay during horizontal approach */

	matrix::Vector2f _sp_pev;
	matrix::Vector2f _sp_pev_prev;

	PrecLandState _state{PrecLandState::Start};

	PrecLandMode _mode{PrecLandMode::Opportunistic};

	bool _is_activated {false}; /**< indicates if precland is activated */

	DEFINE_PARAMETERS(
		(ParamFloat<px4::params::PLD_BTOUT>) _param_pld_btout,
		(ParamFloat<px4::params::PLD_HACC_RAD>) _param_pld_hacc_rad,
		(ParamFloat<px4::params::PLD_FAPPR_ALT>) _param_pld_fappr_alt,
		(ParamFloat<px4::params::PLD_SRCH_ALT>) _param_pld_srch_alt,
		(ParamFloat<px4::params::PLD_SRCH_TOUT>) _param_pld_srch_tout,
		(ParamInt<px4::params::PLD_MAX_SRCH>) _param_pld_max_srch,
		// ly_vision_landing: 本项目末段参数集合，复用MPC_LAND_ALT3；参数作用限于激活PrecLand后的末段。
		(ParamInt<px4::params::PLD_XY_GATE>) _param_pld_xy_gate,
		(ParamFloat<px4::params::PLD_XY_STOP>) _param_pld_xy_stop,
		(ParamFloat<px4::params::PLD_XY_GO>) _param_pld_xy_go,
		(ParamFloat<px4::params::PLD_XY_T>) _param_pld_xy_t,
		(ParamFloat<px4::params::PLD_XY_VEL>) _param_pld_xy_vel,
		(ParamFloat<px4::params::PLD_XY_TOUT>) _param_pld_xy_tout,
		(ParamFloat<px4::params::PLD_XY_CMT_ALT>) _param_pld_xy_cmt_alt,
		(ParamFloat<px4::params::MPC_LAND_ALT3>) _param_mpc_land_alt3,
		(ParamInt<px4::params::PLD_YAW_EN>) _param_pld_yaw_en,
		(ParamFloat<px4::params::PLD_YAW_OFF>) _param_pld_yaw_off,
		(ParamFloat<px4::params::PLD_YAW_STOP>) _param_pld_yaw_stop,
		(ParamFloat<px4::params::PLD_YAW_GO>) _param_pld_yaw_go,
		(ParamFloat<px4::params::PLD_YAW_RATE>) _param_pld_yaw_rate,
		(ParamFloat<px4::params::PLD_YAW_VEL>) _param_pld_yaw_vel,
		(ParamFloat<px4::params::MPC_YAWRAUTO_MAX>) _param_mpc_yawrauto_max
	)

	// non-navigator parameters
	param_t	_handle_param_acceleration_hor{PARAM_INVALID};
	param_t	_handle_param_xy_vel_cruise{PARAM_INVALID};
	float	_param_acceleration_hor{0.0f};
	float	_param_xy_vel_cruise{0.0f};

};
