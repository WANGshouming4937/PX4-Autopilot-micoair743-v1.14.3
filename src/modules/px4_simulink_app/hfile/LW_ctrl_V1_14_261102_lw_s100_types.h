/*
 * File: LW_ctrl_V1_14_261102_lw_s100_types.h
 *
 * Code generated for Simulink model 'LW_ctrl_V1_14_261102_lw_s100'.
 *
 * Model version                  : 8.1269
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Mon Nov  2 16:24:25 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_types_h_
#define RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_types_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include <uORB/topics/rc_channels.h>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_angular_velocity.h>
#include <uORB/topics/vehicle_imu_status.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/landing_target_pose.h>
#include <uORB/topics/trajectory_setpoint.h>
#include <uORB/topics/debug_key_value.h>
#include <uORB/topics/takeoff_status.h>
#include <uORB/topics/airspeed_estimator.h>
#include <uORB/topics/vehicle_land_detected.h>
#include <uORB/topics/vehicle_constraints.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/home_position.h>
#include <uORB/topics/auto_landing_detector.h>
#include <uORB/topics/airspeed.h>
#include <uORB/topics/vehicle_global_position.h>
#include <uORB/topics/sensor_gps.h>
#include <uORB/topics/battery_status.h>
#include <uORB/topics/lw_mixer.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/actuator_armed.h>
#ifndef DEFINED_TYPEDEF_FOR_ControlMode_
#define DEFINED_TYPEDEF_FOR_ControlMode_

typedef enum {
  ARM = 0,                             /* Default value */
  DISARM,
  MANUAL,
  ACRO,
  STABILIZED,
  ALTITUDE,
  POSITION,
  RTL,
  LANDING,
  LOITER,
  MISSION
} ControlMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_offboard_bus_
#define DEFINED_TYPEDEF_FOR_offboard_bus_

typedef struct {
  real32_T pos[3];
  real32_T vel[3];
  real32_T acc[3];
  real32_T q[4];
  real32_T f_d;
  real32_T alpha;
  real32_T beta;
  real32_T rates[3];
  real32_T dotrates[3];
  int8_T OB_TASK;
} offboard_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_control_flag_bus_
#define DEFINED_TYPEDEF_FOR_control_flag_bus_

typedef struct {
  boolean_T manual_enabled;
  boolean_T rates_enabled;
  boolean_T altitude_enabled;
  boolean_T velocity_enabled;
  boolean_T position_enabled;
  boolean_T offboard_enabled;
  boolean_T auto_enabled;
  boolean_T coorturn_enable;
  boolean_T aeroforce_comp_enable;
  boolean_T id_wx_enable;
  boolean_T landing_enable;
} control_flag_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_BusActuatorParam_
#define DEFINED_TYPEDEF_FOR_BusActuatorParam_

typedef struct {
  real_T voltage;
  real_T voltageMin;
  real_T voltageMax;
  real_T capacity;
  real_T T_CR;
  real_T PWM_min;
  real_T PWM_max;
  real_T motorJm;
  real_T Cw[3];
  real_T CT;
  real_T Cm;
  real_T Cmf;
  real_T T_Ca;
} BusActuatorParam;

#endif

#ifndef DEFINED_TYPEDEF_FOR_BusAeroParam_
#define DEFINED_TYPEDEF_FOR_BusAeroParam_

typedef struct {
  real_T Rho;
  real_T CL0;
  real_T CLa;
  real_T CDa;
  real_T alpha0_L;
  real_T C_flat;
  real_T CD0;
  real_T k_L;
  real_T k_D;
  real_T Cm0;
  real_T Cma;
  real_T Cm_90;
  real_T alpha0_M;
  real_T k_M;
  real_T Cmq;
  real_T Cmde;
  real_T CY0;
  real_T CYb;
  real_T Cl0;
  real_T Clb;
  real_T Clp;
  real_T Clda;
  real_T Cn0;
  real_T Cnb;
  real_T Cnr;
  real_T Cndr;
  real_T rotor_Kh;
  real_T rotor_Kdp;
  real_T rotor_Kdq;
  real_T rotor_Kdr;
} BusAeroParam;

#endif

#ifndef DEFINED_TYPEDEF_FOR_BusAirframeParam_
#define DEFINED_TYPEDEF_FOR_BusAirframeParam_

typedef struct {
  real_T mass;
  real_T J[9];
  real_T b;
  real_T c;
  real_T S;
  real_T install_angle;
  real_T delta_min[2];
  real_T delta_max[2];
  real_T delta_rate_max;
  real_T M_surf[6];
  real_T delta_a_min;
  real_T delta_e_min;
  real_T delta_r_min;
  real_T delta_a_max;
  real_T delta_e_max;
  real_T delta_r_max;
  real_T N_servos;
  real_T prearm_servo_scale;
  real_T prearm_servo_mix[8];
  real_T rotor_pos[12];
  real_T rotor_axis[12];
  real_T rotor_dir[4];
  real_T N_motors;
} BusAirframeParam;

#endif

#ifndef DEFINED_TYPEDEF_FOR_battery_status_bus_
#define DEFINED_TYPEDEF_FOR_battery_status_bus_

typedef struct {
  real32_T voltage_filtered_v;
  real32_T current_filtered_a;
  boolean_T connected;
} battery_status_bus;

#endif

#ifndef DEFINED_TYPEDEF_FOR_FlightMode_
#define DEFINED_TYPEDEF_FOR_FlightMode_

typedef enum {
  PILOT = 0,                           /* Default value */
  AUTO,
  OFFBOARD
} FlightMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_YawMode_
#define DEFINED_TYPEDEF_FOR_YawMode_

typedef enum {
  ALIGN_HEADING = 0,                   /* Default value */
  EXT_SET
} YawMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_NavigationState_
#define DEFINED_TYPEDEF_FOR_NavigationState_

typedef enum {
  NAVIGATION_STATE_MANUAL = 0,         /* Default value */
  NAVIGATION_STATE_ALTCTL = 1,
  NAVIGATION_STATE_POSCTL = 2,
  NAVIGATION_STATE_AUTO_MISSION = 3,
  NAVIGATION_STATE_AUTO_LOITER = 4,
  NAVIGATION_STATE_AUTO_RTL = 5,
  NAVIGATION_STATE_UNUSED3 = 8,
  NAVIGATION_STATE_UNUSED = 9,
  NAVIGATION_STATE_ACRO = 10,
  NAVIGATION_STATE_UNUSED1 = 11,
  NAVIGATION_STATE_DESCEND = 12,
  NAVIGATION_STATE_TERMINATION = 13,
  NAVIGATION_STATE_OFFBOARD = 14,
  NAVIGATION_STATE_STAB = 15,
  NAVIGATION_STATE_UNUSED2 = 16,
  NAVIGATION_STATE_AUTO_TAKEOFF = 17,
  NAVIGATION_STATE_AUTO_LAND = 18,
  NAVIGATION_STATE_AUTO_FOLLOW_TARGET = 19,
  NAVIGATION_STATE_AUTO_PRECLAND = 20,
  NAVIGATION_STATE_ORBIT = 21,
  NAVIGATION_STATE_AUTO_VTOL_TAKEOFF = 22,
  NAVIGATION_STATE_MAX = 23
} NavigationState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_FMapMode_
#define DEFINED_TYPEDEF_FOR_FMapMode_

typedef enum {
  FEEDFORWARD_COMP = 0,                /* Default value */
  DF_SOLVER,
  IGNORE_AEROFORCE
} FMapMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_dsoDDCbG6lOspjz09FdzNG_
#define DEFINED_TYPEDEF_FOR_struct_dsoDDCbG6lOspjz09FdzNG_

typedef struct {
  real_T timestamp_us;
  ControlMode arm;
  real32_T throttle[16];
} struct_dsoDDCbG6lOspjz09FdzNG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_J4fIUeiTPqnOTTazeA1WjG_
#define DEFINED_TYPEDEF_FOR_struct_J4fIUeiTPqnOTTazeA1WjG_

typedef struct {
  real32_T stick_xyzr[4];
  real32_T euler_ref[3];
  real32_T rates_ref[3];
  real32_T thrust;
  real32_T delta[3];
  real32_T vel_ref[3];
} struct_J4fIUeiTPqnOTTazeA1WjG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_HxXI1LpsncOaUEy9ysW6PC_
#define DEFINED_TYPEDEF_FOR_struct_HxXI1LpsncOaUEy9ysW6PC_

typedef struct {
  struct_J4fIUeiTPqnOTTazeA1WjG manual;
  offboard_bus offboard;
} struct_HxXI1LpsncOaUEy9ysW6PC;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_rw7aE1NLZapLUkSwZkp1BF_
#define DEFINED_TYPEDEF_FOR_struct_rw7aE1NLZapLUkSwZkp1BF_

typedef struct {
  real32_T pitch;
  real32_T roll;
  real32_T throttle;
  real32_T yaw;
  real32_T ch5;
  real32_T ch6;
  real32_T ch7;
  real32_T ch8;
  real32_T ch9;
  boolean_T signal_lost;
} struct_rw7aE1NLZapLUkSwZkp1BF;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_CjY1fy1HPVSMolCSwj6aIH_
#define DEFINED_TYPEDEF_FOR_struct_CjY1fy1HPVSMolCSwj6aIH_

typedef struct {
  ControlMode ctrl_mode;
  FlightMode flight_mode;
  ControlMode arm;
  control_flag_bus ctrl_flag;
} struct_CjY1fy1HPVSMolCSwj6aIH;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_ziTSObQDNHOjNnaR0C3XAH_
#define DEFINED_TYPEDEF_FOR_struct_ziTSObQDNHOjNnaR0C3XAH_

typedef struct {
  real_T timestamp;
  real32_T vehicle_local_pos[3];
  real32_T vehicle_local_vel[3];
  real32_T delta_xy[2];
  uint8_T xy_reset_counter;
  real32_T delta_z;
  uint8_T z_reset_counter;
  real32_T delta_vxy[2];
  uint8_T vxy_reset_counter;
  real32_T delta_vz;
  uint8_T vz_reset_counter;
  real32_T heading;
  real32_T delta_heading;
  uint8_T heading_reset_counter;
  boolean_T heading_good_for_control;
  boolean_T xy_valid;
  boolean_T z_valid;
  boolean_T v_xy_valid;
  boolean_T v_z_valid;
  real32_T z_deriv;
  real32_T dist_bottom;
  boolean_T dist_bottom_valid;
  uint8_T dist_bottom_sensor_bitfield;
  real_T home_position_timestamp;
  uint8_T home_position_valid_alt;
  real32_T home_position_xyz[3];
} struct_ziTSObQDNHOjNnaR0C3XAH;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_gDk09gFVy103iMmBhaQ0OF_
#define DEFINED_TYPEDEF_FOR_struct_gDk09gFVy103iMmBhaQ0OF_

typedef struct {
  real32_T indicated_airspeed_m_s;
  real32_T true_airspeed_m_s;
  real32_T air_temperature_celsius;
  real32_T confidence;
  real32_T incid[2];
  real32_T v_wind[3];
} struct_gDk09gFVy103iMmBhaQ0OF;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_wPrJgae4BgL1jqYfGk3yYG_
#define DEFINED_TYPEDEF_FOR_struct_wPrJgae4BgL1jqYfGk3yYG_

typedef struct {
  real32_T q[4];
  real32_T delta_q_reset[4];
  uint8_T quat_reset_counter;
  real32_T wb[3];
  real_T timestamp;
} struct_wPrJgae4BgL1jqYfGk3yYG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_mEIg3OMBptiV8K0odN0KFH_
#define DEFINED_TYPEDEF_FOR_struct_mEIg3OMBptiV8K0odN0KFH_

typedef struct {
  real_T lat;
  real_T lon;
  real32_T alt;
  real32_T delta_alt;
  real32_T vel_n_m_s;
  real32_T vel_e_m_s;
  real32_T vel_d_m_s;
  real32_T cog_rad;
} struct_mEIg3OMBptiV8K0odN0KFH;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_9JuccZf9FV5ZrDLPUmlE0E_
#define DEFINED_TYPEDEF_FOR_struct_9JuccZf9FV5ZrDLPUmlE0E_

typedef struct {
  real_T timestamp;
  real32_T gyro_rad[3];
  real32_T accelerometer_m_s2[3];
  real32_T gyro_vibration_metric;
  real32_T accel_vibration_metric;
} struct_9JuccZf9FV5ZrDLPUmlE0E;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_lPTrHq5lSanv8m8JPj5ZpG_
#define DEFINED_TYPEDEF_FOR_struct_lPTrHq5lSanv8m8JPj5ZpG_

typedef struct {
  real_T timestamp;
  boolean_T armed;
  uint8_T nav_state;
  uint8_T takeoff_state;
  real32_T tilt_limit;
  boolean_T ground_contact;
  boolean_T maybe_landed;
  boolean_T landed;
  boolean_T in_ground_effect;
  boolean_T at_rest;
  boolean_T freefall;
  real32_T eso_lambda_xy;
  boolean_T eso_contact_suspected;
  boolean_T want_takeoff;
} struct_lPTrHq5lSanv8m8JPj5ZpG;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_RAu4wxREsEyiECZG2dM7TE_
#define DEFINED_TYPEDEF_FOR_struct_RAu4wxREsEyiECZG2dM7TE_

typedef struct {
  real32_T position[3];
  real32_T velocity[3];
  real32_T acceleration[3];
  real32_T jerk[3];
  real32_T yaw;
  real32_T yawspeed;
} struct_RAu4wxREsEyiECZG2dM7TE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_UdvJxfROCo5pt2wE91SRZE_
#define DEFINED_TYPEDEF_FOR_struct_UdvJxfROCo5pt2wE91SRZE_

typedef struct {
  real_T timestamp;
  boolean_T abs_pos_valid;
  real32_T pos_abs[3];
  real32_T yaw;
} struct_UdvJxfROCo5pt2wE91SRZE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_S7sS0nS95pf2mOkxmYXNiG_
#define DEFINED_TYPEDEF_FOR_struct_S7sS0nS95pf2mOkxmYXNiG_

typedef struct {
  struct_ziTSObQDNHOjNnaR0C3XAH vehicle_local_position;
  struct_gDk09gFVy103iMmBhaQ0OF airspeed;
  struct_wPrJgae4BgL1jqYfGk3yYG vehicle_attitude;
  struct_mEIg3OMBptiV8K0odN0KFH vehicle_global_position;
  battery_status_bus battery_status;
  real32_T T_limit[16];
  struct_9JuccZf9FV5ZrDLPUmlE0E sensor_imu;
  real32_T states_flag;
  struct_lPTrHq5lSanv8m8JPj5ZpG vehicle_status;
  struct_RAu4wxREsEyiECZG2dM7TE trajectory_setpoint;
  struct_UdvJxfROCo5pt2wE91SRZE landing_target;
} struct_S7sS0nS95pf2mOkxmYXNiG;

#endif

/* Custom Type definition for MATLAB Function: '<S115>/Lowpass3' */
#ifndef struct_tag_5BmzoZjf6C8k6vWwNuFVwH
#define struct_tag_5BmzoZjf6C8k6vWwNuFVwH

struct tag_5BmzoZjf6C8k6vWwNuFVwH
{
  real32_T Alpha;
  real32_T State;
  boolean_T IsInitialized;
};

#endif                                 /* struct_tag_5BmzoZjf6C8k6vWwNuFVwH */

#ifndef typedef_LowPassFilter1p_LW_ctrl_V1_14_T
#define typedef_LowPassFilter1p_LW_ctrl_V1_14_T

typedef struct tag_5BmzoZjf6C8k6vWwNuFVwH LowPassFilter1p_LW_ctrl_V1_14_T;

#endif                             /* typedef_LowPassFilter1p_LW_ctrl_V1_14_T */

/* Custom Type definition for MATLAB Function: '<S23>/DisturbanceEstimator' */
#ifndef struct_tag_FQKgh0XGFvFDcRfaFCZizC
#define struct_tag_FQKgh0XGFvFDcRfaFCZizC

struct tag_FQKgh0XGFvFDcRfaFCZizC
{
  real32_T a1;
  real32_T a2;
  real32_T b0;
  real32_T b1;
  real32_T b2;
  real32_T Delay1;
  real32_T Delay2;
  boolean_T IsInitialized;
};

#endif                                 /* struct_tag_FQKgh0XGFvFDcRfaFCZizC */

#ifndef typedef_LowPassFilter2p_LW_ctrl_V1_14_T
#define typedef_LowPassFilter2p_LW_ctrl_V1_14_T

typedef struct tag_FQKgh0XGFvFDcRfaFCZizC LowPassFilter2p_LW_ctrl_V1_14_T;

#endif                             /* typedef_LowPassFilter2p_LW_ctrl_V1_14_T */

#ifndef struct_tag_yLAc11DdrbocDWwkqJQgrE
#define struct_tag_yLAc11DdrbocDWwkqJQgrE

struct tag_yLAc11DdrbocDWwkqJQgrE
{
  LowPassFilter2p_LW_ctrl_V1_14_T FilterX;
  LowPassFilter2p_LW_ctrl_V1_14_T FilterY;
  LowPassFilter2p_LW_ctrl_V1_14_T FilterZ;
};

#endif                                 /* struct_tag_yLAc11DdrbocDWwkqJQgrE */

#ifndef typedef_LowPassFilter2pVector3_LW_ctr_T
#define typedef_LowPassFilter2pVector3_LW_ctr_T

typedef struct tag_yLAc11DdrbocDWwkqJQgrE LowPassFilter2pVector3_LW_ctr_T;

#endif                             /* typedef_LowPassFilter2pVector3_LW_ctr_T */

/* Custom Type definition for MATLAB Function: '<S33>/velocity_pid' */
#ifndef struct_tag_0HgFH0wpJCOB1wLRyy3dr
#define struct_tag_0HgFH0wpJCOB1wLRyy3dr

struct tag_0HgFH0wpJCOB1wLRyy3dr
{
  real32_T SampleTime;
  uint8_T IntegratorMethod;
  uint8_T FilterMethod;
  uint8_T AntiWindupMethod;
  real32_T BackCalculationGain;
  real32_T IntegratorInitialCondition;
  real32_T FilterInitialCondition;
  real32_T IntegratorState;
  real32_T FilterState;
  real32_T PreviousIntegratorInput;
  real32_T PreviousDerivativeOutput;
};

#endif                                 /* struct_tag_0HgFH0wpJCOB1wLRyy3dr */

#ifndef typedef_RflyDiscretePID_LW_ctrl_V1_14_T
#define typedef_RflyDiscretePID_LW_ctrl_V1_14_T

typedef struct tag_0HgFH0wpJCOB1wLRyy3dr RflyDiscretePID_LW_ctrl_V1_14_T;

#endif                             /* typedef_RflyDiscretePID_LW_ctrl_V1_14_T */

#ifndef struct_tag_sKv95S4FhAbO2Chqql86NVD
#define struct_tag_sKv95S4FhAbO2Chqql86NVD

struct tag_sKv95S4FhAbO2Chqql86NVD
{
  real32_T P;
  real32_T b_I;
  real32_T D;
  real32_T Unsaturated;
  real32_T Output;
  real32_T IntegratorInput;
  real32_T IntegratorState;
  real32_T FilterState;
  boolean_T IsOutputLimited;
};

#endif                                 /* struct_tag_sKv95S4FhAbO2Chqql86NVD */

#ifndef typedef_sKv95S4FhAbO2Chqql86NVD_LW_ct_T
#define typedef_sKv95S4FhAbO2Chqql86NVD_LW_ct_T

typedef struct tag_sKv95S4FhAbO2Chqql86NVD sKv95S4FhAbO2Chqql86NVD_LW_ct_T;

#endif                             /* typedef_sKv95S4FhAbO2Chqql86NVD_LW_ct_T */

/* Custom Type definition for MATLAB Function: '<S32>/MATLAB Function1' */
#ifndef struct_tag_1dOHVraNeklUc2yTMKe5n
#define struct_tag_1dOHVraNeklUc2yTMKe5n

struct tag_1dOHVraNeklUc2yTMKe5n
{
  real32_T xw_prev[3];
  real32_T yb_prev[3];
  real32_T zw_prev[3];
  real32_T R_prev[9];
  real32_T alpha_prev;
  real32_T beta_prev;
  boolean_T initialized;
};

#endif                                 /* struct_tag_1dOHVraNeklUc2yTMKe5n */

#ifndef typedef_liftwingFlatnessSolver_LW_ctr_T
#define typedef_liftwingFlatnessSolver_LW_ctr_T

typedef struct tag_1dOHVraNeklUc2yTMKe5n liftwingFlatnessSolver_LW_ctr_T;

#endif                             /* typedef_liftwingFlatnessSolver_LW_ctr_T */

/* Custom Type definition for MATLAB Function: '<S116>/df' */
#ifndef struct_tag_TsVM1PyxkR3vs6yWOiaKbG
#define struct_tag_TsVM1PyxkR3vs6yWOiaKbG

struct tag_TsVM1PyxkR3vs6yWOiaKbG
{
  real32_T xw_prev[3];
  real32_T yb_prev[3];
  real32_T zw_prev[3];
  real32_T R_prev[9];
  real32_T alpha_prev;
  real32_T beta_prev;
  real32_T wb_prev[3];
  boolean_T initialized;
};

#endif                                 /* struct_tag_TsVM1PyxkR3vs6yWOiaKbG */

#ifndef typedef_liftwingFlatnessSolver_LW_c_e_T
#define typedef_liftwingFlatnessSolver_LW_c_e_T

typedef struct tag_TsVM1PyxkR3vs6yWOiaKbG liftwingFlatnessSolver_LW_c_e_T;

#endif                             /* typedef_liftwingFlatnessSolver_LW_c_e_T */

/* Custom Type definition for MATLAB Function: '<S35>/position control' */
#ifndef struct_tag_CpTSb0givrcSqoKupJ7xgH
#define struct_tag_CpTSb0givrcSqoKupJ7xgH

struct tag_CpTSb0givrcSqoKupJ7xgH
{
  uint8_T Mode;
  real32_T max_accel;
  real32_T max_vel;
  real32_T max_jerk;
  real32_T kp_vel_tracking;
  real32_T backward_velocity_scale;
  real32_T low_speed_side_velocity_limit;
  real32_T low_forward_speed_threshold;
  real32_T max_coordinated_turn_rate;
  real32_T cruise_stick_deadzone;
  real32_T cruise_forward_accel_scale;
  real32_T cruise_transition_band;
  real32_T pos_sp[2];
  real32_T vel_sp[2];
  real32_T acc_sp[2];
  real32_T acc_sp_prev[2];
  boolean_T is_holding;
  uint64m_T timestamp_last;
};

#endif                                 /* struct_tag_CpTSb0givrcSqoKupJ7xgH */

#ifndef typedef_TrajectoryGeneratorXY_LW_ctrl_T
#define typedef_TrajectoryGeneratorXY_LW_ctrl_T

typedef struct tag_CpTSb0givrcSqoKupJ7xgH TrajectoryGeneratorXY_LW_ctrl_T;

#endif                             /* typedef_TrajectoryGeneratorXY_LW_ctrl_T */

#ifndef struct_emxArray_real32_T_1x1
#define struct_emxArray_real32_T_1x1

struct emxArray_real32_T_1x1
{
  real32_T data;
  int32_T size[2];
};

#endif                                 /* struct_emxArray_real32_T_1x1 */

#ifndef typedef_emxArray_real32_T_1x1_LW_ctrl_T
#define typedef_emxArray_real32_T_1x1_LW_ctrl_T

typedef struct emxArray_real32_T_1x1 emxArray_real32_T_1x1_LW_ctrl_T;

#endif                             /* typedef_emxArray_real32_T_1x1_LW_ctrl_T */

/* Forward declaration for rtModel */
typedef struct tag_RTM_LW_ctrl_V1_14_261102__T RT_MODEL_LW_ctrl_V1_14_261102_T;

#endif                    /* RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
