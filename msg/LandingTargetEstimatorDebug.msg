# 精简诊断：滤波结果、三轴创新与航向观测结果；不参与控制。
# Snapshot from the actual estimator invocation; diagnostic only.
uint64 timestamp
uint64 observation_timestamp
uint64 observation_timestamp_sample
uint64 source_timestamp # Join with VisionLandingDebug.source_timestamp
uint8 source_system
uint8 source_component
uint8 result # 0 ignored invalid observation, 1 invalid vehicle state, 2 invalid numbers, 3 initialized, 4 accepted, 5 gated, 6 timeout
float32[3] observation_body
float32[3] measurement_ned # NaN if not transformed
float32[3] variance_after # Filtered target position variance, m^2; NaN when uninitialized
float32[3] innovation # Computed on all axes, including gated observations
float32[3] innovation_variance
uint8 heading_result # 0 none, 1 invalid quaternion, 2 bad time, 3 nonhorizontal plane, 4 acquiring, 5 accepted, 6 angle jump
float32 yaw_raw
float32 yaw_attitude_time_error_s # Nearest attitude sample minus observation sample; 0 if interpolated
uint8 ORB_QUEUE_LENGTH = 8
