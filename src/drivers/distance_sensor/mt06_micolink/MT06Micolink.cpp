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

#include "MT06Micolink.hpp"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <drivers/drv_sensor.h>
#include <lib/drivers/device/Device.hpp>
#include <lib/mathlib/mathlib.h>

MT06Micolink::MT06Micolink(const char *port, uint8_t rotation) :
	ScheduledWorkItem(MODULE_NAME, px4::serial_port_to_wq(port)),
	_px4_rangefinder(0, rotation)
{
	strncpy(_port, port, sizeof(_port) - 1);
	_port[sizeof(_port) - 1] = '\0';

	device::Device::DeviceId device_id;
	device_id.devid_s.bus_type = device::Device::DeviceBusType_SERIAL;

	const uint8_t bus_num = atoi(&_port[strlen(_port) - 1]);

	if (bus_num < 10) {
		device_id.devid_s.bus = bus_num;
	}

	_px4_rangefinder.set_device_id(device_id.devid);
	_px4_rangefinder.set_device_type(DRV_DIST_DEVTYPE_MT06_MICOLINK);
	_px4_rangefinder.set_rangefinder_type(distance_sensor_s::MAV_DISTANCE_SENSOR_LASER);
	_px4_rangefinder.set_min_distance(0.01f);
	_px4_rangefinder.set_max_distance(6.0f);
	_px4_rangefinder.set_fov(math::radians(2.0f));
}

MT06Micolink::~MT06Micolink()
{
	stop();

	perf_free(_sample_perf);
	perf_free(_comms_errors);
	perf_free(_bytes_received);
	perf_free(_frames_seen);
	perf_free(_bad_checksum);
	perf_free(_bad_msgid);
	perf_free(_bad_len);
	perf_free(_invalid_distance);
	perf_free(_tof_status_nonzero);
	perf_free(_valid_frame);
}

int MT06Micolink::init()
{
	start();
	return PX4_OK;
}

int MT06Micolink::open_serial_port(speed_t speed)
{
	if (_fd >= 0) {
		return PX4_OK;
	}

	_fd = ::open(_port, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (_fd < 0) {
		PX4_ERR("failed to open %s", _port);
		return PX4_ERROR;
	}

	termios uart_config {};

	if (tcgetattr(_fd, &uart_config) < 0) {
		PX4_ERR("tcgetattr failed");
		::close(_fd);
		_fd = -1;
		return PX4_ERROR;
	}

	cfsetispeed(&uart_config, speed);
	cfsetospeed(&uart_config, speed);

	uart_config.c_cflag |= (CLOCAL | CREAD);
	uart_config.c_cflag &= ~CSIZE;
	uart_config.c_cflag |= CS8;
	uart_config.c_cflag &= ~PARENB;
	uart_config.c_cflag &= ~CSTOPB;
	uart_config.c_cflag &= ~CRTSCTS;
	uart_config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	uart_config.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	uart_config.c_oflag &= ~OPOST;
	uart_config.c_cc[VMIN] = 0;
	uart_config.c_cc[VTIME] = 1;

	if (tcsetattr(_fd, TCSANOW, &uart_config) < 0) {
		PX4_ERR("tcsetattr failed");
		::close(_fd);
		_fd = -1;
		return PX4_ERROR;
	}

	return PX4_OK;
}

void MT06Micolink::start()
{
	ScheduleOnInterval(kConversionInterval);
}

void MT06Micolink::stop()
{
	ScheduleClear();

	if (_fd >= 0) {
		::close(_fd);
		_fd = -1;
	}
}

void MT06Micolink::Run()
{
	if (_fd < 0) {
		open_serial_port();
		return;
	}

	collect();
}

int MT06Micolink::collect()
{
	perf_begin(_sample_perf);

	uint8_t readbuf[64] {};
	const ssize_t bytes_read = ::read(_fd, readbuf, sizeof(readbuf));

	if (bytes_read < 0) {
		const int read_errno = errno;
		_last_read_errno = read_errno;

		if (read_errno != EAGAIN) {
			perf_count(_comms_errors);

			if (read_errno == EBADF) {
				_fd = -1;
			}

			const hrt_abstime now = hrt_absolute_time();

			if (now - _last_debug_print > 1_s) {
				_last_debug_print = now;
				PX4_INFO("read failed errno=%d fd=%d port=%s", read_errno, _fd, _port);
			}
		}

		perf_end(_sample_perf);
		return PX4_OK;
	}


	if (bytes_read == 0) {
		perf_end(_sample_perf);
		return PX4_OK;
	}

	for (ssize_t i = 0; i < bytes_read; i++) {
		perf_count(_bytes_received);
	}

	_last_read = hrt_absolute_time();

	for (ssize_t i = 0; i < bytes_read; i++) {
		float distance_m = NAN;
		int8_t quality = -1;

		if (parse_byte(readbuf[i], distance_m, quality)) {
			perf_count(_valid_frame);
			_px4_rangefinder.update(_last_read, distance_m, quality);
		}
	}

	perf_end(_sample_perf);
	return PX4_OK;
}

bool MT06Micolink::parse_byte(uint8_t byte, float &distance_m, int8_t &quality)
{
	switch (_parse_state) {
	case ParserState::Header:
		if (byte == FRAME_HEADER) {
			_frame_index = 0;
			_frame[_frame_index++] = byte;
			_parse_state = ParserState::DevId;
		}

		break;

	case ParserState::DevId:
		_frame[_frame_index++] = byte;
		_parse_state = ParserState::SysId;
		break;

	case ParserState::SysId:
		_frame[_frame_index++] = byte;
		_parse_state = ParserState::MessageId;
		break;

	case ParserState::MessageId:
		_frame[_frame_index++] = byte;
		_parse_state = ParserState::Seq;
		break;

	case ParserState::Seq:
		_frame[_frame_index++] = byte;
		_parse_state = ParserState::Length;
		break;

	case ParserState::Length:
		_payload_len = byte;

		if (_payload_len > MAX_PAYLOAD_LEN) {
			_parse_state = ParserState::Header;

		} else {
			_frame[_frame_index++] = byte;
			_parse_state = (_payload_len == 0) ? ParserState::Checksum : ParserState::Payload;
		}

		break;

	case ParserState::Payload:
		_frame[_frame_index++] = byte;

		if (_frame_index == HEADER_LEN + _payload_len) {
			_parse_state = ParserState::Checksum;
		}

		break;

	case ParserState::Checksum:
		_frame[_frame_index++] = byte;
		_parse_state = ParserState::Header;

		return decode_frame(distance_m, quality);
	}

	return false;
}

bool MT06Micolink::decode_frame(float &distance_m, int8_t &quality)
{
	perf_count(_frames_seen);

	uint8_t checksum = 0;
	const size_t frame_len = HEADER_LEN + _payload_len + 1;

	for (size_t i = 0; i < frame_len - 1; i++) {
		checksum += _frame[i];
	}

	if (checksum != _frame[frame_len - 1]) {
		perf_count(_comms_errors);
		perf_count(_bad_checksum);
		print_frame_debug("bad_checksum", frame_len);
		return false;
	}

	if (_frame[3] != MESSAGE_ID) {
		perf_count(_bad_msgid);
		print_frame_debug("bad_msgid", frame_len);
		return false;
	}

	// Micolink range payload:
	// uint32 time_ms, uint32 distance_mm, uint8 strength, uint8 precision,
	// uint8 tof_status, uint8 reserved, int16 flow_vel_x, int16 flow_vel_y,
	// uint8 flow_quality, uint8 flow_status, uint16 reserved2.
	if (_payload_len < RANGE_SENSOR_PAYLOAD_LEN) {
		perf_count(_comms_errors);
		perf_count(_bad_len);
		print_frame_debug("bad_len", frame_len);
		return false;
	}

	const uint8_t *payload = &_frame[HEADER_LEN];
	const uint32_t distance_mm = static_cast<uint32_t>(payload[RANGE_SENSOR_DISTANCE_OFFSET])
				     | (static_cast<uint32_t>(payload[RANGE_SENSOR_DISTANCE_OFFSET + 1]) << 8)
				     | (static_cast<uint32_t>(payload[RANGE_SENSOR_DISTANCE_OFFSET + 2]) << 16)
				     | (static_cast<uint32_t>(payload[RANGE_SENSOR_DISTANCE_OFFSET + 3]) << 24);
	const uint8_t strength = payload[RANGE_SENSOR_STRENGTH_OFFSET];
	const uint8_t tof_status = payload[RANGE_SENSOR_TOF_STATUS_OFFSET];

	// Micolink defines distance == 0 as unavailable. Do not publish it as 0 m.
	if (distance_mm == 0) {
		perf_count(_invalid_distance);
		print_frame_debug("invalid_distance", frame_len);
		return false;
	}

	if (tof_status != 0) {
		perf_count(_tof_status_nonzero);
		print_frame_debug("nonzero_status", frame_len);
	}

	distance_m = static_cast<float>(distance_mm) * 0.001f;
	quality = strength_to_quality(strength);

	return PX4_ISFINITE(distance_m);
}

int8_t MT06Micolink::strength_to_quality(uint16_t strength) const
{
	if (strength == 0) {
		return -1;
	}

	const int quality = math::constrain(static_cast<int>(strength * 100U / 255U), 1, 100);
	return static_cast<int8_t>(quality);
}

void MT06Micolink::print_frame_debug(const char *reason, size_t frame_len)
{
	const hrt_abstime now = hrt_absolute_time();

	// Limit console spam. One raw-frame print per second is enough for diagnosis.
	if (now - _last_debug_print < 1_s) {
		return;
	}

	_last_debug_print = now;

	const size_t print_len = math::min(frame_len, static_cast<size_t>(24));
	char hex[24 * 3 + 1] {};

	for (size_t i = 0; i < print_len; i++) {
		snprintf(&hex[i * 3], sizeof(hex) - i * 3, "%02x ", _frame[i]);
	}

	PX4_INFO("debug %s len=%u payload_len=%u msgid=0x%02x checksum=0x%02x frame: %s",
		 reason,
		 static_cast<unsigned>(frame_len),
		 static_cast<unsigned>(_payload_len),
		 static_cast<unsigned>(_frame[3]),
		 static_cast<unsigned>(_frame[frame_len - 1]),
		 hex);
}

void MT06Micolink::print_status_debug()
{
	perf_print_counter(_bytes_received);
	perf_print_counter(_frames_seen);
	perf_print_counter(_bad_checksum);
	perf_print_counter(_bad_msgid);
	perf_print_counter(_bad_len);
	perf_print_counter(_invalid_distance);
	perf_print_counter(_tof_status_nonzero);
	perf_print_counter(_valid_frame);
}

void MT06Micolink::print_info()
{
	perf_print_counter(_sample_perf);
	perf_print_counter(_comms_errors);
	print_status_debug();
	PX4_INFO("port: %s fd: %d last_read_errno: %d", _port, _fd, _last_read_errno);
}
