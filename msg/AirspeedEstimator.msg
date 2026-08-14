#airspeed estimator

uint64 timestamp		    # time since system start (microseconds)
#uint64 timestamp_sample     # the timestamp of the raw data (microseconds)



float32[3] v_wind_est
float32 airspeed_est

float32 update_enable
float32 ekf_update
float32 algebraic_valid
float32 airspeed_pred
float32 airspeed_alg
float32 innov_norm
float32 nis
float32 cond_h
