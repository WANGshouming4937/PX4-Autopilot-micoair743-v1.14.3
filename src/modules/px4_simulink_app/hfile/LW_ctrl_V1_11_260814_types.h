/*
 * File: LW_ctrl_V1_11_260814_types.h
 *
 * Code generated for Simulink model 'LW_ctrl_V1_11_260814'.
 *
 * Model version                  : 8.1146
 * Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
 * C/C++ source code generated on : Fri Aug 14 14:51:27 2026
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_LW_ctrl_V1_11_260814_types_h_
#define RTW_HEADER_LW_ctrl_V1_11_260814_types_h_
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
#include <uORB/topics/vehicle_local_position.h>
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

#ifndef DEFINED_TYPEDEF_FOR_VehicleType_
#define DEFINED_TYPEDEF_FOR_VehicleType_

typedef enum {
  LIFTING_WING = 0,                    /* Default value */
  MULTICOPTER,
  FIXED_WING
} VehicleType;

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
  real_T Cw[2];
  real_T CT;
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

#ifndef DEFINED_TYPEDEF_FOR_struct_H0yvT6Hapl8LsZ8iZLgoiB_
#define DEFINED_TYPEDEF_FOR_struct_H0yvT6Hapl8LsZ8iZLgoiB_

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
} struct_H0yvT6Hapl8LsZ8iZLgoiB;

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

#ifndef DEFINED_TYPEDEF_FOR_struct_4j4suW4uoI17BrMTdlRdGG_
#define DEFINED_TYPEDEF_FOR_struct_4j4suW4uoI17BrMTdlRdGG_

typedef struct {
  real_T timestamp;
  boolean_T armed;
  uint8_T nav_state;
  uint8_T takeoff_state;
  real32_T tilt_limit;
} struct_4j4suW4uoI17BrMTdlRdGG;

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

#ifndef DEFINED_TYPEDEF_FOR_struct_sF74AK6JcPBDMkGXE3ZmgD_
#define DEFINED_TYPEDEF_FOR_struct_sF74AK6JcPBDMkGXE3ZmgD_

typedef struct {
  struct_H0yvT6Hapl8LsZ8iZLgoiB vehicle_local_position;
  struct_gDk09gFVy103iMmBhaQ0OF airspeed;
  struct_wPrJgae4BgL1jqYfGk3yYG vehicle_attitude;
  struct_mEIg3OMBptiV8K0odN0KFH vehicle_global_position;
  battery_status_bus battery_status;
  real32_T T_limit[16];
  struct_9JuccZf9FV5ZrDLPUmlE0E sensor_imu;
  real32_T states_flag;
  struct_4j4suW4uoI17BrMTdlRdGG vehicle_status;
  struct_RAu4wxREsEyiECZG2dM7TE trajectory_setpoint;
  struct_UdvJxfROCo5pt2wE91SRZE landing_target;
} struct_sF74AK6JcPBDMkGXE3ZmgD;

#endif

/* Custom Type definition for MATLAB Function: '<S92>/IntegratedMixer' */
#ifndef struct_tag_sRQ09YwUNBScnorFf9oRpHD
#define struct_tag_sRQ09YwUNBScnorFf9oRpHD

struct tag_sRQ09YwUNBScnorFf9oRpHD
{
  real_T xstar[5];
  real_T fstar;
  real_T firstorderopt;
  real_T lambda[9];
  int32_T state;
  real_T maxConstr;
  int32_T iterations;
  real_T searchDir[5];
};

#endif                                 /* struct_tag_sRQ09YwUNBScnorFf9oRpHD */

#ifndef typedef_sRQ09YwUNBScnorFf9oRpHD_LW_ct_T
#define typedef_sRQ09YwUNBScnorFf9oRpHD_LW_ct_T

typedef struct tag_sRQ09YwUNBScnorFf9oRpHD sRQ09YwUNBScnorFf9oRpHD_LW_ct_T;

#endif                             /* typedef_sRQ09YwUNBScnorFf9oRpHD_LW_ct_T */

#ifndef struct_tag_szdozZxcClCeUpd1TsA9nJF
#define struct_tag_szdozZxcClCeUpd1TsA9nJF

struct tag_szdozZxcClCeUpd1TsA9nJF
{
  real_T grad[5];
  real_T Hx[4];
  boolean_T hasLinear;
  int32_T nvar;
  int32_T maxVar;
  real_T beta;
  real_T rho;
  int32_T objtype;
  int32_T prev_objtype;
  int32_T prev_nvar;
  boolean_T prev_hasLinear;
  real_T gammaScalar;
};

#endif                                 /* struct_tag_szdozZxcClCeUpd1TsA9nJF */

#ifndef typedef_szdozZxcClCeUpd1TsA9nJF_LW_ct_T
#define typedef_szdozZxcClCeUpd1TsA9nJF_LW_ct_T

typedef struct tag_szdozZxcClCeUpd1TsA9nJF szdozZxcClCeUpd1TsA9nJF_LW_ct_T;

#endif                             /* typedef_szdozZxcClCeUpd1TsA9nJF_LW_ct_T */

#ifndef struct_tag_sY034dY3cx1IgSIMa0jtlQE
#define struct_tag_sY034dY3cx1IgSIMa0jtlQE

struct tag_sY034dY3cx1IgSIMa0jtlQE
{
  real_T FMat[25];
  int32_T ldm;
  int32_T ndims;
  int32_T info;
  real_T scaleFactor;
  boolean_T ConvexCheck;
  real_T regTol_;
  real_T workspace_[240];
  real_T workspace2_[240];
};

#endif                                 /* struct_tag_sY034dY3cx1IgSIMa0jtlQE */

#ifndef typedef_sY034dY3cx1IgSIMa0jtlQE_LW_ct_T
#define typedef_sY034dY3cx1IgSIMa0jtlQE_LW_ct_T

typedef struct tag_sY034dY3cx1IgSIMa0jtlQE sY034dY3cx1IgSIMa0jtlQE_LW_ct_T;

#endif                             /* typedef_sY034dY3cx1IgSIMa0jtlQE_LW_ct_T */

#ifndef struct_tag_sL9bDKomAYkxZSVrG9w6En
#define struct_tag_sL9bDKomAYkxZSVrG9w6En

struct tag_sL9bDKomAYkxZSVrG9w6En
{
  int32_T MaxIterations;
  real_T ConstrRelTolFactor;
  real_T ProbRelTolFactor;
  boolean_T RemainFeasible;
};

#endif                                 /* struct_tag_sL9bDKomAYkxZSVrG9w6En */

#ifndef typedef_sL9bDKomAYkxZSVrG9w6En_LW_ctr_T
#define typedef_sL9bDKomAYkxZSVrG9w6En_LW_ctr_T

typedef struct tag_sL9bDKomAYkxZSVrG9w6En sL9bDKomAYkxZSVrG9w6En_LW_ctr_T;

#endif                             /* typedef_sL9bDKomAYkxZSVrG9w6En_LW_ctr_T */

/* Custom Type definition for MATLAB Function: '<S21>/DisturbanceEstimator' */
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

#ifndef typedef_LowPassFilter2p_LW_ctrl_V1_11_T
#define typedef_LowPassFilter2p_LW_ctrl_V1_11_T

typedef struct tag_FQKgh0XGFvFDcRfaFCZizC LowPassFilter2p_LW_ctrl_V1_11_T;

#endif                             /* typedef_LowPassFilter2p_LW_ctrl_V1_11_T */

#ifndef struct_tag_yLAc11DdrbocDWwkqJQgrE
#define struct_tag_yLAc11DdrbocDWwkqJQgrE

struct tag_yLAc11DdrbocDWwkqJQgrE
{
  LowPassFilter2p_LW_ctrl_V1_11_T FilterX;
  LowPassFilter2p_LW_ctrl_V1_11_T FilterY;
  LowPassFilter2p_LW_ctrl_V1_11_T FilterZ;
};

#endif                                 /* struct_tag_yLAc11DdrbocDWwkqJQgrE */

#ifndef typedef_LowPassFilter2pVector3_LW_ctr_T
#define typedef_LowPassFilter2pVector3_LW_ctr_T

typedef struct tag_yLAc11DdrbocDWwkqJQgrE LowPassFilter2pVector3_LW_ctr_T;

#endif                             /* typedef_LowPassFilter2pVector3_LW_ctr_T */

/* Custom Type definition for MATLAB Function: '<S114>/Lowpass3' */
#ifndef struct_tag_5BmzoZjf6C8k6vWwNuFVwH
#define struct_tag_5BmzoZjf6C8k6vWwNuFVwH

struct tag_5BmzoZjf6C8k6vWwNuFVwH
{
  real32_T Alpha;
  real32_T State;
  boolean_T IsInitialized;
};

#endif                                 /* struct_tag_5BmzoZjf6C8k6vWwNuFVwH */

#ifndef typedef_LowPassFilter1p_LW_ctrl_V1_11_T
#define typedef_LowPassFilter1p_LW_ctrl_V1_11_T

typedef struct tag_5BmzoZjf6C8k6vWwNuFVwH LowPassFilter1p_LW_ctrl_V1_11_T;

#endif                             /* typedef_LowPassFilter1p_LW_ctrl_V1_11_T */

/* Custom Type definition for MATLAB Function: '<S31>/velocity_pid' */
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

#ifndef typedef_RflyDiscretePID_LW_ctrl_V1_11_T
#define typedef_RflyDiscretePID_LW_ctrl_V1_11_T

typedef struct tag_0HgFH0wpJCOB1wLRyy3dr RflyDiscretePID_LW_ctrl_V1_11_T;

#endif                             /* typedef_RflyDiscretePID_LW_ctrl_V1_11_T */

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

/* Custom Type definition for MATLAB Function: '<S92>/IntegratedMixer' */
#ifndef struct_tag_sYWi7HrsYZnVmS9q0papwvE
#define struct_tag_sYWi7HrsYZnVmS9q0papwvE

struct tag_sYWi7HrsYZnVmS9q0papwvE
{
  real_T workspace_double[45];
  int32_T workspace_int[9];
  int32_T workspace_sort[9];
  real_T workspace_compareIneq[45];
};

#endif                                 /* struct_tag_sYWi7HrsYZnVmS9q0papwvE */

#ifndef typedef_sYWi7HrsYZnVmS9q0papwvE_LW_ct_T
#define typedef_sYWi7HrsYZnVmS9q0papwvE_LW_ct_T

typedef struct tag_sYWi7HrsYZnVmS9q0papwvE sYWi7HrsYZnVmS9q0papwvE_LW_ct_T;

#endif                             /* typedef_sYWi7HrsYZnVmS9q0papwvE_LW_ct_T */

#ifndef struct_tag_s78GCJvmtD1Rws5pzEtzVXF
#define struct_tag_s78GCJvmtD1Rws5pzEtzVXF

struct tag_s78GCJvmtD1Rws5pzEtzVXF
{
  int32_T ldq;
  real_T QR[45];
  real_T Q[25];
  int32_T jpvt[9];
  int32_T mrows;
  int32_T ncols;
  real_T tau[5];
  int32_T minRowCol;
  boolean_T usedPivoting;
};

#endif                                 /* struct_tag_s78GCJvmtD1Rws5pzEtzVXF */

#ifndef typedef_s78GCJvmtD1Rws5pzEtzVXF_LW_ct_T
#define typedef_s78GCJvmtD1Rws5pzEtzVXF_LW_ct_T

typedef struct tag_s78GCJvmtD1Rws5pzEtzVXF s78GCJvmtD1Rws5pzEtzVXF_LW_ct_T;

#endif                             /* typedef_s78GCJvmtD1Rws5pzEtzVXF_LW_ct_T */

/* Custom Type definition for MATLAB Function: '<S30>/MATLAB Function1' */
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

/* Custom Type definition for MATLAB Function: '<S115>/df' */
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

/* Custom Type definition for MATLAB Function: '<S33>/position control' */
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

/* Custom Type definition for MATLAB Function: '<S92>/IntegratedMixer' */
#ifndef struct_tag_sSOeZ9WO10chPn9Si6PKiCB
#define struct_tag_sSOeZ9WO10chPn9Si6PKiCB

struct tag_sSOeZ9WO10chPn9Si6PKiCB
{
  real_T InitDamping;
  char_T FiniteDifferenceType[7];
  boolean_T SpecifyObjectiveGradient;
  boolean_T ScaleProblem;
  boolean_T SpecifyConstraintGradient;
  boolean_T NonFiniteSupport;
  boolean_T IterDisplaySQP;
  real_T FiniteDifferenceStepSize;
  real_T MaxFunctionEvaluations;
  boolean_T IterDisplayQP;
  real_T PricingTolerance;
  char_T Algorithm[10];
  real_T ObjectiveLimit;
  real_T ConstraintTolerance;
  real_T OptimalityTolerance;
  real_T StepTolerance;
  real_T MaxIterations;
  real_T FunctionTolerance;
  char_T SolverName[8];
  boolean_T CheckGradients;
  char_T Diagnostics[3];
  real_T DiffMaxChange;
  real_T DiffMinChange;
  char_T Display[5];
  char_T FunValCheck[3];
  boolean_T UseParallel;
  char_T LinearSolver[4];
  char_T SubproblemAlgorithm[2];
};

#endif                                 /* struct_tag_sSOeZ9WO10chPn9Si6PKiCB */

#ifndef typedef_sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T
#define typedef_sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T

typedef struct tag_sSOeZ9WO10chPn9Si6PKiCB sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T;

#endif                             /* typedef_sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T */

#ifndef struct_tag_sTTULnc01OVJqZqpgkzM1HG
#define struct_tag_sTTULnc01OVJqZqpgkzM1HG

struct tag_sTTULnc01OVJqZqpgkzM1HG
{
  int32_T mConstr;
  int32_T mConstrOrig;
  int32_T mConstrMax;
  int32_T nVar;
  int32_T nVarOrig;
  int32_T nVarMax;
  int32_T ldA;
  real_T lb[5];
  real_T ub[5];
  int32_T indexLB[5];
  int32_T indexUB[5];
  int32_T indexFixed[5];
  int32_T mEqRemoved;
  real_T ATwset[45];
  real_T bwset[9];
  int32_T nActiveConstr;
  real_T maxConstrWorkspace[9];
  int32_T sizes[5];
  int32_T sizesNormal[5];
  int32_T sizesPhaseOne[5];
  int32_T sizesRegularized[5];
  int32_T sizesRegPhaseOne[5];
  int32_T isActiveIdx[6];
  int32_T isActiveIdxNormal[6];
  int32_T isActiveIdxPhaseOne[6];
  int32_T isActiveIdxRegularized[6];
  int32_T isActiveIdxRegPhaseOne[6];
  boolean_T isActiveConstr[9];
  int32_T Wid[9];
  int32_T Wlocalidx[9];
  int32_T nWConstr[5];
  int32_T probType;
  real_T SLACK0;
};

#endif                                 /* struct_tag_sTTULnc01OVJqZqpgkzM1HG */

#ifndef typedef_sTTULnc01OVJqZqpgkzM1HG_LW_ct_T
#define typedef_sTTULnc01OVJqZqpgkzM1HG_LW_ct_T

typedef struct tag_sTTULnc01OVJqZqpgkzM1HG sTTULnc01OVJqZqpgkzM1HG_LW_ct_T;

#endif                             /* typedef_sTTULnc01OVJqZqpgkzM1HG_LW_ct_T */

#ifndef struct_tag_Y0fZiQeiQNtYbm9KDBAmGB
#define struct_tag_Y0fZiQeiQNtYbm9KDBAmGB

struct tag_Y0fZiQeiQNtYbm9KDBAmGB
{
  real_T X[4];
  sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T Options;
  real_T MaxLinearEqualities;
  real_T MaxLinearInequalities;
  sRQ09YwUNBScnorFf9oRpHD_LW_ct_T solution;
  sYWi7HrsYZnVmS9q0papwvE_LW_ct_T memspace;
  sTTULnc01OVJqZqpgkzM1HG_LW_ct_T WorkingSet;
  szdozZxcClCeUpd1TsA9nJF_LW_ct_T QPObjective;
  s78GCJvmtD1Rws5pzEtzVXF_LW_ct_T QRManager;
  sY034dY3cx1IgSIMa0jtlQE_LW_ct_T CholRegManager;
  sL9bDKomAYkxZSVrG9w6En_LW_ctr_T runTimeOptions;
};

#endif                                 /* struct_tag_Y0fZiQeiQNtYbm9KDBAmGB */

#ifndef typedef_c_optim_coder_warmstart_Quadp_T
#define typedef_c_optim_coder_warmstart_Quadp_T

typedef struct tag_Y0fZiQeiQNtYbm9KDBAmGB c_optim_coder_warmstart_Quadp_T;

#endif                             /* typedef_c_optim_coder_warmstart_Quadp_T */

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
typedef struct tag_RTM_LW_ctrl_V1_11_260814_T RT_MODEL_LW_ctrl_V1_11_260814_T;

#endif                            /* RTW_HEADER_LW_ctrl_V1_11_260814_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
