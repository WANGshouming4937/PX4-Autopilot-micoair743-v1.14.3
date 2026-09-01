#pragma once

#include <float.h>
#include <matrix/matrix/helper_functions.hpp>
#include "VelocitySmoothing.hpp"

class HeadingSmoothing
{
public:
	HeadingSmoothing();
	~HeadingSmoothing() = default;

	void setMaxHeadingRate(const float max_heading_rate)
	{
		_velocity_smoothing.setMaxAccel(max_heading_rate);
	}

	void setMaxHeadingAccel(const float max_heading_accel)
	{
		_velocity_smoothing.setMaxJerk(max_heading_accel);
	}

	void reset(const float heading, const float heading_rate = NAN);

	void update(const float heading_setpoint, const float time_elapsed);

	float getSmoothedHeading() const
	{
		return _velocity_smoothing.getCurrentVelocity();
	}

	float getSmoothedHeadingRate() const
	{
		return _velocity_smoothing.getCurrentAcceleration();
	}

private:
	VelocitySmoothing _velocity_smoothing;
};
