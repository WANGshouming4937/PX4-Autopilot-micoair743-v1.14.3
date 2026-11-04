/****************************************************************************
 * Copyright (c) 2026 PX4 Development Team. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 ****************************************************************************/
#pragma once

#include <matrix/math.hpp>
#include <cmath>
#include <cstdint>

/** Independent orientation path. It never modifies the XYZ filter or camera lever arm. */
// ly_vision_landing: 新增独立靶标航向通道：历史机体姿态 × PnP完整姿态得到NED纸张前向，不改XYZ或相机杆臂。
class LandingTargetHeading
{
public:

	enum class Result : uint8_t { None = 0, InvalidQuaternion, BadTime, BadPlane, Acquiring, Accepted, AngleJump };

	static float wrap(float angle) { return atan2f(sinf(angle), cosf(angle)); }
	// ly_vision_landing: 原地清空姿态历史，避免在H743的小任务栈上构造整个历史缓存的临时副本。
	void reset()
	{
		// Reset in place: assigning a temporary copy of the complete attitude
		// history used more than 600 bytes of stack on the H743 toolchain.
		for (auto &sample : _history) {
			sample.timestamp = 0;
			sample.q = matrix::Quatf{};
		}

		_next = 0;
		_last_attitude_time = 0;
		_reset_counter = 0;
		_have_reset_counter = false;
		_initialized = false;
		_yaw = NAN;
		_raw_yaw = NAN;
		_attitude_time_error = NAN;
		_last_observation = 0;
		_candidate_start = 0;
		_candidate_last = 0;
		_candidate_count = 0;
		_candidate_anchor = 0.f;
		_candidate_mean = 0.f;
	}

	void pushAttitude(uint64_t timestamp, const float q[4], uint8_t reset_counter, uint64_t now)
	{
		if (_have_reset_counter && reset_counter != _reset_counter) { reset(); }
		_have_reset_counter = true;
		_reset_counter = reset_counter;
		if (timestamp == 0 || timestamp > now || now - timestamp > 500000) { return; }
		matrix::Quatf quaternion(q);
		if (!normalize(quaternion)) { return; }
		if (_last_attitude_time == timestamp) { return; }
		_last_attitude_time = timestamp;
		_history[_next].timestamp = timestamp;
		_history[_next].q = quaternion;
		_next = (_next + 1) % HistoryLength;
	}

	// ly_vision_landing: 拒绝过期/无法匹配时刻/法向不符的方向；初次需至少4帧且覆盖0.3秒，已捕获后拒绝超过20度跳变。
	// ly_vision_landing: 航向按环绕角误差和0.25秒时间常数滤波，0.5秒无接受观测失效；不是直接追单帧yaw。
	Result observe(const float body_target_q[4], uint64_t sample_time, uint64_t now)
	{
		matrix::Quatf body_target(body_target_q);
		if (!normalize(body_target)) { return Result::InvalidQuaternion; }
		if (sample_time == 0 || sample_time > now || now - sample_time > 250000) { return Result::BadTime; }

		matrix::Quatf attitude;
		if (!attitudeAt(sample_time, attitude)) { return Result::BadTime; }
		const matrix::Dcmf target_ned(attitude * body_target);
		// A printed airport marker is horizontal. Reject plausible-reprojection
		// planar PnP flips which tilt its upward-facing normal more than 30 deg.
		// NED down is positive: the printed face normal must point UP. This also
		// rejects legacy yaw-only quaternions which omit the marker's 180-deg roll.
		if (target_ned(2, 2) > -0.8660254f || hypotf(target_ned(0, 0), target_ned(1, 0)) < 0.8f) {
			return Result::BadPlane;
		}

		_raw_yaw = atan2f(target_ned(1, 0), target_ned(0, 0));
		if (!valid(now)) { _initialized = false; }

		if (!_initialized) {
			if (_candidate_start == 0 || sample_time <= _candidate_last
			    || sample_time - _candidate_last > 250000 || fabsf(wrap(_raw_yaw - _candidate_anchor)) > 0.2617994f) {
				_candidate_start = sample_time;
				_candidate_anchor = _raw_yaw;
				_candidate_mean = _raw_yaw;
				_candidate_count = 0;
			}

			_candidate_last = sample_time;
			++_candidate_count;
			_candidate_mean = wrap(_candidate_mean + wrap(_raw_yaw - _candidate_mean) / _candidate_count);
			if (_candidate_count < 4 || sample_time - _candidate_start < 300000) { return Result::Acquiring; }
			_yaw = _candidate_mean;
			_initialized = true;

		} else {
			const float error = wrap(_raw_yaw - _yaw);
			if (fabsf(error) > 0.3490659f) { return Result::AngleJump; }
			if (sample_time <= _last_observation) { return Result::BadTime; }
			const float dt = fminf((sample_time - _last_observation) * 1e-6f, 0.25f);
			const float alpha = 1.f - expf(-dt / 0.25f);
			_yaw = wrap(_yaw + alpha * error);
		}

		_last_observation = sample_time;
		return Result::Accepted;
	}

	bool valid(uint64_t now) const
	{
		return _initialized && _last_observation > 0 && now >= _last_observation && now - _last_observation <= 500000;
	}
	float yaw() const { return _yaw; }
	float rawYaw() const { return _raw_yaw; }
	uint64_t timestamp() const { return _last_observation; }
	float attitudeTimeError() const { return _attitude_time_error; }

private:
	static constexpr unsigned HistoryLength = 24;
	struct Attitude { uint64_t timestamp{0}; matrix::Quatf q{}; };
	Attitude _history[HistoryLength]{};
	unsigned _next{0};
	uint64_t _last_attitude_time{0};
	uint8_t _reset_counter{0};
	bool _have_reset_counter{false};
	bool _initialized{false};
	float _yaw{NAN};
	float _raw_yaw{NAN};
	float _attitude_time_error{NAN};
	uint64_t _last_observation{0};
	uint64_t _candidate_start{0};
	uint64_t _candidate_last{0};
	unsigned _candidate_count{0};
	float _candidate_anchor{0.f};
	float _candidate_mean{0.f};

	static bool normalize(matrix::Quatf &q)
	{
		const float norm = q.norm();
		if (!q.isAllFinite() || !std::isfinite(norm) || norm < 0.5f || norm > 1.5f) { return false; }
		q /= norm;
		return true;
	}

	// ly_vision_landing: 在24条姿态历史内插值或选最近样本，时间差必须在100 ms内；不能消除未知曝光/驱动延迟。
	bool attitudeAt(uint64_t sample_time, matrix::Quatf &q)
	{
		const Attitude *before = nullptr;
		const Attitude *after = nullptr;
		for (const auto &sample : _history) {
			if (sample.timestamp == 0) { continue; }
			if (sample.timestamp <= sample_time && (!before || sample.timestamp > before->timestamp)) { before = &sample; }
			if (sample.timestamp >= sample_time && (!after || sample.timestamp < after->timestamp)) { after = &sample; }
		}

		if (before && after && before->timestamp != after->timestamp
		    && sample_time - before->timestamp <= 100000 && after->timestamp - sample_time <= 100000) {
			const float fraction = float(sample_time - before->timestamp) / float(after->timestamp - before->timestamp);
			matrix::Quatf end = after->q;
			if (before->q.dot(end) < 0.f) { end *= -1.f; }
			for (int i = 0; i < 4; ++i) { q(i) = (1.f - fraction) * before->q(i) + fraction * end(i); }
			_attitude_time_error = 0.f;
			return normalize(q);
		}

		const Attitude *nearest = before;
		if (!nearest || (after && after->timestamp - sample_time < sample_time - nearest->timestamp)) { nearest = after; }
		if (!nearest) { return false; }
		const int64_t difference = int64_t(nearest->timestamp) - int64_t(sample_time);
		_attitude_time_error = difference * 1e-6f;
		if (difference < -100000 || difference > 100000) { return false; }
		q = nearest->q;
		return true;
	}
};
