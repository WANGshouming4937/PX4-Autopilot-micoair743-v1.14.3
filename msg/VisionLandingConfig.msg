# ly_vision_landing: 只读配置快照：标定、尺寸、几何偏移、外参和脚本哈希，供离线重放；不用于在线控制。
# Repeated metadata from Pi; protocol VLCFG1 / array_id 31002.
uint64 timestamp
uint64 source_timestamp
uint32 session_id
uint8 source_system
uint8 source_component
uint16 image_width
uint16 image_height
int16 marker_id_large
int16 marker_id_small
float32[9] camera_matrix
float32[5] distortion
float32 marker_length_large
float32 marker_length_small
float32[3] landing_offset_large
float32[3] landing_offset_small
float32[9] rotation_camera_to_body
float32[3] translation_camera_to_body
uint8[32] script_sha256
