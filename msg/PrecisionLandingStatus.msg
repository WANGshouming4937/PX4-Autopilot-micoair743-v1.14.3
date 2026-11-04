# Read-only snapshot after PrecLand on_active state handling, and activation/deactivation.
uint64 timestamp
uint8 state # 0 Start, 1 HorizontalApproach, 2 DescendAboveTarget, 3 FinalApproach, 4 Search, 5 Fallback, 6 Done
uint8 mode # 1 opportunistic, 2 required
bool active
bool target_abs_valid
bool landed
float32 horizontal_error
bool xy_gate_enabled
bool xy_gate_active
bool descent_paused
bool xy_gate_timed_out
bool touchdown_committed
uint8 pause_reason # 0 none, 1 XY error, 2 settling, 3 target invalid, 4 height invalid, 5 vehicle invalid, 6 hold timeout, 7 committed, 8 yaw invalid, 9 yaw error
uint8 height_source # 0 unavailable, 1 vehicle_local_position.dist_bottom, 2 fresh visual target Z minus vehicle Z
float32 surface_distance # Sensor-to-surface distance, not skid clearance
float32 horizontal_speed
float32 aligned_time_s
float32 pause_time_s
bool yaw_enabled
bool yaw_valid
bool yaw_control_active
float32 yaw_error # Wrapped airport-target error, not error to the intermediate rate-limited reference
float32 yaw_rate # Filtered actual heading derivative, rad/s
uint8 ORB_QUEUE_LENGTH = 4
