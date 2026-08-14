#attitude control setpoint

uint64 timestamp		    # time since system start (microseconds)
#uint64 timestamp_sample     # the timestamp of the raw data (microseconds)

float32 delta_t

float32[4] log_ac_att_q
float32[3] log_ac_att_wb

float32[4] log_ac_q_ref
float32[3] log_ac_rates_ref
float32[3] log_ac_thrust_ref
float32[3] log_ac_torque_ref
float32[3] log_ac_d_hat
float32[3] log_ac_rates_est
float32[4] log_ac_q_est

