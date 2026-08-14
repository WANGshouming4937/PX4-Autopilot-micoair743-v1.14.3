#position control setpoint

uint64 timestamp		    # time since system start (microseconds)
#uint64 timestamp_sample     # the timestamp of the raw data (microseconds)

float32 delta_t

float32[6] log_pc_local_pos
float32[6] log_pc_local_pos_ref
float32[4] log_pc_att_ref
float32[3] log_pc_accel_ref
float32[3] log_pc_ae_hat
float32[3] log_pc_vel_est
float32[3] log_pc_pos_est
float32[3] ae_bias_hover

