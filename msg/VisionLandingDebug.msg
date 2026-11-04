# Diagnostic only. Does not supply control inputs. Protocol VLDBG1 / array_id 31001.
uint64 timestamp # PX4 reception time [us]
uint64 source_timestamp # Exact LANDING_TARGET.time_usec, source wall clock [us]; join key
uint64 timestamp_send_estimate # Pi estimate of PX4 time at control send start; zero if unsynced
uint64 timestamp_read_end_estimate # Approximate cap.read return in PX4 time; NOT exposure time
uint32 frame_sequence
uint32 session_id # Random per Python process
int16 marker_id # -1 when no selected marker
uint8 source_system
uint8 source_component
uint8 marker_type # 0 none, 1 large, 2 small
uint8 reason # 0 valid, 1 no fresh frame, 2 wrong size, 3 no marker, 4 PnP failure, 5 stale
uint8 detect_mode # 0 none, 1 high-full, 2 low-full, 3 large-roi, 4 small-roi
bool control_send_success
bool clock_valid # Midpoint TIMESYNC estimate, inspect RTT/age before latency analysis
float32 processing_ms # Processing start to LANDING_TARGET send start
float32 ready_to_send_ms
float32 clock_sample_age_ms
float32 clock_rtt_ms
uint8 ORB_QUEUE_LENGTH = 8
