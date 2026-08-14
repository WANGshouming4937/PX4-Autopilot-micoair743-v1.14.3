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

#include <termios.h>

#include <drivers/drv_hrt.h>
#include <lib/drivers/rangefinder/PX4Rangefinder.hpp>
#include <lib/perf/perf_counter.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/topics/distance_sensor.h>

using namespace time_literals;

#define MT06_MICOLINK_DEFAULT_PORT "/dev/ttyS3"

class MT06Micolink : public px4::ScheduledWorkItem
{
public:
	MT06Micolink(const char *port, uint8_t rotation = distance_sensor_s::ROTATION_DOWNWARD_FACING);
	~MT06Micolink() override;

	int init();
	void print_info();

private:
	static constexpr uint8_t FRAME_HEADER = 0xEF;
	static constexpr uint8_t MESSAGE_ID = 0x51;
	static constexpr size_t HEADER_LEN = 6;
	static constexpr size_t MAX_PAYLOAD_LEN = 64;
	static constexpr size_t MAX_FRAME_LEN = HEADER_LEN + MAX_PAYLOAD_LEN + 1;
	static constexpr size_t RANGE_SENSOR_PAYLOAD_LEN = 20;
	static constexpr size_t RANGE_SENSOR_DISTANCE_OFFSET = 4;
	static constexpr size_t RANGE_SENSOR_STRENGTH_OFFSET = 8;
	static constexpr size_t RANGE_SENSOR_TOF_STATUS_OFFSET = 10;
	static constexpr int kConversionInterval{20_ms};

	enum class ParserState {
		Header,
		DevId,
		SysId,
		MessageId,
		Seq,
		Length,
		Payload,
		Checksum
	};

	int collect();
	void Run() override;
	void start();
	void stop();

	bool parse_byte(uint8_t byte, float &distance_m, int8_t &quality);
	bool decode_frame(float &distance_m, int8_t &quality);
	int open_serial_port(speed_t speed = B115200);
	int8_t strength_to_quality(uint16_t strength) const;
	void print_frame_debug(const char *reason, size_t frame_len);
	void print_status_debug();

	PX4Rangefinder _px4_rangefinder;

	ParserState _parse_state{ParserState::Header};
	uint8_t _frame[MAX_FRAME_LEN] {};
	size_t _frame_index{0};
	uint8_t _payload_len{0};

	char _port[20] {};
	int _fd{-1};
	int _last_read_errno{0};
	hrt_abstime _last_read{0};
	hrt_abstime _last_debug_print{0};

	perf_counter_t _comms_errors{perf_alloc(PC_COUNT, MODULE_NAME": com_err")};
	perf_counter_t _sample_perf{perf_alloc(PC_ELAPSED, MODULE_NAME": read")};
	perf_counter_t _bytes_received{perf_alloc(PC_COUNT, MODULE_NAME": bytes")};
	perf_counter_t _frames_seen{perf_alloc(PC_COUNT, MODULE_NAME": frames_seen")};
	perf_counter_t _bad_checksum{perf_alloc(PC_COUNT, MODULE_NAME": bad_checksum")};
	perf_counter_t _bad_msgid{perf_alloc(PC_COUNT, MODULE_NAME": bad_msgid")};
	perf_counter_t _bad_len{perf_alloc(PC_COUNT, MODULE_NAME": bad_len")};
	perf_counter_t _invalid_distance{perf_alloc(PC_COUNT, MODULE_NAME": invalid_distance")};
	perf_counter_t _tof_status_nonzero{perf_alloc(PC_COUNT, MODULE_NAME": tof_status_nonzero")};
	perf_counter_t _valid_frame{perf_alloc(PC_COUNT, MODULE_NAME": valid_frame")};
};
