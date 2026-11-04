/****************************************************************************
 * Copyright (c) 2026 PX4 Development Team. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 ****************************************************************************/
#pragma once

#include <cmath>
#include <cstdint>

/** Deterministic terminal-descent policy; references are held by PrecLand. */
// ly_vision_landing: 本项目新增末段下降判据，只输出暂停/恢复/承诺状态；坐标与轨迹由PrecLand和FlightTaskAuto管理。
// ly_vision_landing: 复用 MPC_LAND_ALT3 启用高度，不新建位置PID；所有高度是传感器/视觉参考点离地，不是脚架间隙。
class PrecisionLandingGate
{
public:

	enum class Reason : uint8_t {
		None = 0,
		HorizontalError = 1,
		Settling = 2,
		TargetInvalid = 3,
		HeightInvalid = 4,
		VehicleInvalid = 5,
		Timeout = 6,
		Committed = 7,
		YawInvalid = 8,
		YawError = 9
	};

	struct Config {
		bool enabled{true};
		float gate_alt{1.f}; // MPC_LAND_ALT3, distance above the landing surface
		float commit_alt{0.20f}; // Same sensor-to-surface convention, NOT skid clearance
		float stop_error{0.05f};
		float resume_error{0.03f};
		float settle_time{0.5f};
		float resume_speed{0.10f};
		float hold_timeout{10.f};
		bool yaw_enabled{false};
		float yaw_stop{0.17453293f};
		float yaw_resume{0.08726646f};
		float yaw_settle_rate{0.08726646f};
		float yaw_rate_limit{0.26179939f};
	};

	struct Input {
		uint64_t now{0};
		float height{NAN};
		float horizontal_error{NAN};
		float horizontal_speed{NAN};
		bool target_valid{false};
		bool vehicle_valid{false};
		bool yaw_valid{false};
		float yaw_error{NAN};
		float yaw_rate{NAN};
	};

	void reset() { *this = PrecisionLandingGate{}; }

	void update(const Config &config, const Input &input)
	{
		// Once committed, target loss, height bounce and parameter changes must not
		// restart horizontal pursuit. Only mode exit/new activation resets this.
		if (_committed) { return; }

		if (!config.enabled || !std::isfinite(config.gate_alt) || config.gate_alt <= 0.f) {
			reset();
			return;
		}

		const bool height_valid = std::isfinite(input.height) && input.height > 0.f;
		const float stop_error = fmaxf(config.stop_error, 0.01f);
		const float resume_error = fminf(fmaxf(config.resume_error, 0.f), stop_error);
		const float commit_alt = fminf(fmaxf(config.commit_alt, 0.f), config.gate_alt);
		if (!_paused) {
			// Reserve the time needed for the initial turn before counting the
			// existing XY hold timeout. A 180-deg turn at 15 deg/s needs 12 s.
			const float angle = input.yaw_valid && std::isfinite(input.yaw_error) ? fabsf(input.yaw_error) : 3.14159265f;
			_pause_yaw_budget = config.yaw_enabled && config.yaw_rate_limit > 0.f ? angle / config.yaw_rate_limit : 0.f;
		}
		const bool loop_continuous = _last_update > 0 && input.now >= _last_update
					    && input.now - _last_update <= 250000;
		_last_update = input.now;

		if (!loop_continuous) { _aligned_since = 0; }

		// ly_vision_landing: 首次进入末段先暂停，等XY/速度及启用后的yaw均稳定再下降；不是仅首次大误差才检查。
		if (!_active) {
			if (height_valid && input.height > config.gate_alt) { return; }

			// An unknown height cannot authorize continued descent.
			_active = true;
			pause(input.now);
		}

		// Timeout is latched until mode exit. It must not silently become a
		// normal landing or resume merely because a target reappears.
		// ly_vision_landing: 等待超时锁存，不能靠目标重现或高度回跳自行恢复；模式退出/重新激活清除，原生接管和失效保护仍在外层。
		if (_timed_out) { return; }

		// This is the explicit last-centimetres exception requested by the user.
		// Below it, commit even if the target is lost or XY error has increased.
		// Never commit on invalid vehicle state or an invalid height.
		// ly_vision_landing: 承诺高度是明确的近地例外：有效高度/飞机状态下允许丢靶仍落地；此前已超时锁存则保持等待。
		if (input.vehicle_valid && height_valid && commit_alt > 0.f && input.height <= commit_alt) {
			_committed = true;
			_paused = false;
			_aligned_since = 0;
			_reason = Reason::Committed;
			return;
		}

		if (!input.vehicle_valid) {
			pause(input.now);
			_reason = Reason::VehicleInvalid;

		} else if (!height_valid) {
			pause(input.now);
			_reason = Reason::HeightInvalid;

		} else if (!input.target_valid || !std::isfinite(input.horizontal_error)) {
			pause(input.now);
			_reason = Reason::TargetInvalid;

		} else if (input.horizontal_error > stop_error) {
			pause(input.now);
			_reason = Reason::HorizontalError;

		} else if (config.yaw_enabled && (!input.yaw_valid || !std::isfinite(input.yaw_error))) {
			pause(input.now);
			_reason = Reason::YawInvalid;

		} else if (config.yaw_enabled && fabsf(input.yaw_error) > fmaxf(config.yaw_stop, 0.f)) {
			pause(input.now);
			_reason = Reason::YawError;

		// ly_vision_landing: 暂停期间必须在GO门限内且速度/转速合格，连续满足PLD_XY_T才恢复；任一不满足立即重计时。
		// ly_vision_landing: 恢复后只要误差再超过STOP门限仍会暂停，STOP/GO滞回防抖；水平速度门限本身只用于恢复条件。
		} else if (_paused) {
			const bool aligned = input.horizontal_error <= resume_error
					     && std::isfinite(input.horizontal_speed)
					     && input.horizontal_speed <= fmaxf(config.resume_speed, 0.f)
					     && (!config.yaw_enabled || (input.yaw_valid
						&& fabsf(input.yaw_error) <= fminf(fmaxf(config.yaw_resume, 0.f), fmaxf(config.yaw_stop, 0.f))
						&& std::isfinite(input.yaw_rate) && fabsf(input.yaw_rate) <= fmaxf(config.yaw_settle_rate, 0.f)));

			if (!aligned) {
				_aligned_since = 0;

			} else if (_aligned_since == 0) {
				_aligned_since = input.now;
			}

			_reason = Reason::Settling;

			if (aligned && _aligned_since > 0
			    && input.now - _aligned_since >= static_cast<uint64_t>(fmaxf(config.settle_time, 0.f) * 1e6f)) {
				_paused = false;
				_pause_since = 0;
				_aligned_since = 0;
				_reason = Reason::None;
			}

		} else {
			_reason = Reason::None;
		}

		if (_paused && config.hold_timeout > 0.f && input.now >= _pause_since
		    && input.now - _pause_since >= static_cast<uint64_t>((config.hold_timeout + _pause_yaw_budget) * 1e6f)) {
			_timed_out = true;
			_aligned_since = 0;
			_reason = Reason::Timeout;
		}
	}

	bool active() const { return _active; }
	bool paused() const { return _paused; }
	bool committed() const { return _committed; }
	bool timedOut() const { return _timed_out; }
	Reason reason() const { return _reason; }
	float stableTime(uint64_t now) const { return _aligned_since > 0 && now >= _aligned_since ? (now - _aligned_since) * 1e-6f : 0.f; }
	float pauseTime(uint64_t now) const { return _paused && now >= _pause_since ? (now - _pause_since) * 1e-6f : 0.f; }

private:
	void pause(uint64_t now)
	{
		if (!_paused) { _pause_since = now; }

		_paused = true;
		_aligned_since = 0;
	}

	bool _active{false};
	bool _paused{false};
	bool _committed{false};
	bool _timed_out{false};
	Reason _reason{Reason::None};
	uint64_t _last_update{0};
	uint64_t _pause_since{0};
	uint64_t _aligned_since{0};
	float _pause_yaw_budget{0.f};
};
