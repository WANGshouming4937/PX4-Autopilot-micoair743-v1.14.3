# Raw landing-target observation received from an external vision system.
# Position and orientation are relative to the vehicle in MAV_FRAME_BODY_FRD.

uint8 FRAME_BODY_FRD = 12

uint64 timestamp                    # time the observation was received by PX4 [us]
uint64 timestamp_sample             # synchronized source sample time [us]

bool position_valid                 # true when position_body contains a current observation
bool orientation_valid              # true when q_body_target and yaw_body are valid

uint8 frame                         # MAVLink coordinate frame (MAV_FRAME_BODY_FRD expected)
uint8 target_num                    # MAVLink landing target identifier

float32[3] position_body            # forward, right, down position relative to vehicle [m]
float32[4] q_body_target            # target orientation relative to body, quaternion w, x, y, z
float32 yaw_body                    # target yaw relative to body [rad]
