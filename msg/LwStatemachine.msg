#state machine

uint64 timestamp		    # time since system start (microseconds)
#uint64 timestamp_sample     # the timestamp of the raw data (microseconds)

float32 delta_t

uint8 flight_mode
uint8 control_mode
uint8 arm

uint8 manual_enabled
uint8 rates_enabled
uint8 altitude_enabled
uint8 velocity_enabled
uint8 position_enabled
uint8 offboard_enabled
uint8 auto_enabled
uint8 coorturn_enable
uint8 aeroforce_comp_enable
uint8 id_wx_enable

