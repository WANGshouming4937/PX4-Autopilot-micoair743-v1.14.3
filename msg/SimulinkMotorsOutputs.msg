# This message contains the motors outputs from the Simulink controller.
# The outputs are expected to be in the range [0, 1].

uint64 timestamp          # time since system start (microseconds)

bool arm                  # Arming command from Simulink (see safety note below)

# Define a constant for the number of outputs.
uint8 NUM_ACTUATORS = 8

# Actuator motors outputs, range [0, 1]
float32[8] actuator_motors_outputs
