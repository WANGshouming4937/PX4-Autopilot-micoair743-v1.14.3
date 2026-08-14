#pragma once

#include "FunctionProviderBase.hpp"
#include <uORB/SubscriptionCallback.hpp>
#include <uORB/topics/simulink_motors_outputs.h> // 包含自定义的消息头文件
#include <px4_platform_common/defines.h>   // For PX4_ISFINITE

#define CONFIG_ARCH_BOARD_PX4_SIMULINK_HIL
/**
 * @class FunctionSimulink
 * Functions: Simulink_Motor1 ... Simulink_MotorMax
 * Implements a function provider that subscribes to the 'simulink_motor_outputs' uORB topic
 * and provides the actuator values to the MixingOutput module.
 *
 * The entire implementation is contained within this header file for simplicity.
 */
class FunctionSimulinkMotors : public FunctionProviderBase
{
public:
	/**
	 * Constructor.
	 * @param context Provides access to the work item for the subscription callback.
	 */
	FunctionSimulinkMotors(const Context &context) :
		// Initialize the subscription callback. This links our function provider
		// to the MixingOutput's work queue, making it event-driven.
		_topic(&context.work_item, ORB_ID(simulink_motors_outputs))
	{
		// Initialize local data copy with invalid values for safety.
		for (int i = 0; i < simulink_motors_outputs_s::NUM_ACTUATORS; ++i) {
			_data.actuator_motors_outputs[i] = NAN;
		}
	}

	// Default destructor.
	virtual ~FunctionSimulinkMotors() = default;

	/**
	 * Factory method required by MixingOutput to create an instance of this class.
	 */
	static FunctionProviderBase *allocate(const Context &context)
	{
		return new FunctionSimulinkMotors(context);
	}

	/**
	 * This method is called by the scheduler when a new 'simulink_motors_outputs' message arrives.
	 * It copies the new data into our local '_data' buffer.
	 */
	void update() override
	{
		_topic.update(&_data);
	}

	/**
	 * This method is called by MixingOutput to get the control value for a specific actuator.
	 * @param func The specific function (e.g., Simulink_Motor1) being requested.
	 * @return A float value in the range [-1, 1].
	 */
	float value(OutputFunction func) override
	{
		// Calculate the array index from the OutputFunction enum value.
		int index = (int)func - (int)OutputFunction::Simulink_Motor1;

		// Ensure the index is within the bounds of our data array.
		if (index >= 0 && index < simulink_motors_outputs_s::NUM_ACTUATORS) {

			// Retrieve the control value (expected range [0, 1]) from our local data.
			const float output_0_to_1 = _data.actuator_motors_outputs[index];

			// The MixingOutput module requires a normalized value in the range [-1, 1]
			// to correctly map it to the configured MIN/MAX PWM values.
			// We must convert our [0, 1] signal to this [-1, 1] range.
			if (PX4_ISFINITE(output_0_to_1)) {
				return (output_0_to_1 * 2.f) - 1.f;
			}
		}

		// If the index is out of bounds or the value is not finite, return NAN
		// to signal that this output should be set to its disarmed value.
		return NAN;
	}

	/**
	 * Provide the subscription callback item to MixingOutput's scheduler.
	 */
	uORB::SubscriptionCallbackWorkItem *subscriptionCallback() override { return &_topic; }

	/**
	 * Provide the timestamp of the last received message for latency tracking.
	 */
	bool getLatestSampleTimestamp(hrt_abstime &t) const override
	{
		t = _data.timestamp;
		return (t != 0);
	}

	// Motors are generally not controlled before the system is fully armed.
	bool allowPrearmControl() const override { return false; }

	// Assuming the Simulink controller outputs are for non-reversible motors.
	// If your motors are reversible, change this to `return true;`.
	bool reversible(OutputFunction func) const override { return false; }

private:
	uORB::SubscriptionCallbackWorkItem _topic; // The uORB subscription object.
	simulink_motors_outputs_s              _data{};  // Local buffer for the latest message data.
};
