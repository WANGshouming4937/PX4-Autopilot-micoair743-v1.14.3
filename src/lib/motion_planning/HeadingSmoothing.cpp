#include "HeadingSmoothing.hpp"

HeadingSmoothing::HeadingSmoothing()
{
	_velocity_smoothing.setMaxVel(M_TWOPI_F);
}

void HeadingSmoothing::reset(const float heading, const float heading_rate)
{
	const float wrapped_heading = matrix::wrap_pi(heading);
	_velocity_smoothing.setCurrentVelocity(wrapped_heading);

	if (PX4_ISFINITE(heading_rate)) {
		_velocity_smoothing.setCurrentAcceleration(heading_rate);
	}
}

void HeadingSmoothing::update(const float heading_setpoint, const float time_elapsed)
{
	const float delta_heading_wrapped =
		matrix::wrap_pi(heading_setpoint - getSmoothedHeading());

	const float unwrapped_heading_setpoint =
		delta_heading_wrapped + getSmoothedHeading();

	_velocity_smoothing.updateDurations(unwrapped_heading_setpoint);
	_velocity_smoothing.updateTraj(time_elapsed);

	const float wrapped_current_heading =
		matrix::wrap_pi(getSmoothedHeading());

	_velocity_smoothing.setCurrentVelocity(wrapped_current_heading);
}
