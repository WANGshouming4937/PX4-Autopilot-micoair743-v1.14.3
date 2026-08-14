/*
 * File: LW_ctrl_V1_11_260814.h
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

#ifndef RTW_HEADER_LW_ctrl_V1_11_260814_h_
#define RTW_HEADER_LW_ctrl_V1_11_260814_h_
#ifndef LW_ctrl_V1_11_260814_COMMON_INCLUDES_
#define LW_ctrl_V1_11_260814_COMMON_INCLUDES_
#include <uORB/uORB.h>
#include <poll.h>
#include <stdio.h>
#include <px4_platform_common/log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nuttx/config.h>
#include <nuttx/sched.h>
#include <systemlib/err.h>
#include <uORB/topics/lw_mixer.h>
#include <uORB/topics/lw_attctrl.h>
#include <uORB/topics/debug_array.h>
#include <uORB/topics/takeoff_status.h>
#include <uORB/topics/vehicle_land_detected.h>
#include <uORB/topics/auto_landing_detector.h>
#include <uORB/topics/airspeed_estimator.h>
#include <uORB/topics/lw_posctrl.h>
#include <uORB/topics/vehicle_local_position_setpoint.h>
#include <uORB/topics/lw_navigator.h>
#include <uORB/topics/vision_landing.h>
#include <uORB/topics/lw_statemachine.h>
#include <uORB/topics/manual_control_setpoint.h>
#include <uORB/topics/rc_channels.h>
#include <uORB/topics/lw_manualctrl.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_angular_velocity.h>
#include <uORB/topics/sensor_gps.h>
#include <uORB/topics/battery_status.h>
#include <uORB/topics/vehicle_imu_status.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/actuator_armed.h>
#include <uORB/topics/trajectory_setpoint.h>
#include <uORB/topics/debug_key_value.h>
#include <uORB/topics/landing_target_pose.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/simulink_motors_outputs.h>
#include <uORB/topics/simulink_servos_outputs.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                               /* LW_ctrl_V1_11_260814_COMMON_INCLUDES_ */

#include "LW_ctrl_V1_11_260814_types.h"
#include "multiword_types.h"
#include "rt_nonfinite.h"
#include <stddef.h>
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include <math.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

typedef struct lw_mixer_s lw_mixer_s_t;
typedef struct lw_attctrl_s lw_attctrl_s_t;
typedef struct debug_array_s debug_array_s_t;
typedef struct takeoff_status_s takeoff_status_s_t;
typedef struct vehicle_land_detected_s vehicle_land_detected_s_t;
typedef struct auto_landing_detector_s auto_landing_detector_s_t;
typedef struct airspeed_estimator_s airspeed_estimator_s_t;
typedef struct lw_posctrl_s lw_posctrl_s_t;
typedef struct vehicle_local_position_setpoint_s
  vehicle_local_position_setpoint_s_t;
typedef struct lw_navigator_s lw_navigator_s_t;
typedef struct vision_landing_s vision_landing_s_t;
typedef struct lw_statemachine_s lw_statemachine_s_t;
typedef struct pollfd pollfd_t;
typedef struct manual_control_setpoint_s manual_control_setpoint_SL;
typedef struct pollfd pollfd_t;
typedef struct rc_channels_s rc_channels_SL;
typedef struct lw_manualctrl_s lw_manualctrl_s_t;
typedef struct pollfd pollfd_t;
typedef struct vehicle_local_position_s vehicle_local_position_SL;
typedef struct pollfd pollfd_t;
typedef struct vehicle_attitude_s vehicle_attitude_SL;
typedef struct pollfd pollfd_t;
typedef struct airspeed_estimator_s airspeed_estimator_SL;
typedef struct pollfd pollfd_t;
typedef struct vehicle_angular_velocity_s vehicle_angular_velocity_SL;
typedef struct pollfd pollfd_t;
typedef struct sensor_gps_s sensor_gps_SL;
typedef struct pollfd pollfd_t;
typedef struct battery_status_s battery_status_SL;
typedef struct pollfd pollfd_t;
typedef struct lw_mixer_s lw_mixer_SL;
typedef struct pollfd pollfd_t;
typedef struct vehicle_imu_status_s vehicle_imu_status_SL;
typedef struct pollfd pollfd_t;
typedef struct sensor_combined_s sensor_combined_SL;
typedef struct pollfd pollfd_t;
typedef struct vehicle_status_s vehicle_status_SL;
typedef struct pollfd pollfd_t;
typedef struct takeoff_status_s takeoff_status_SL;
typedef struct pollfd pollfd_t;
typedef struct actuator_armed_s actuator_armed_SL;
typedef struct pollfd pollfd_t;
typedef struct trajectory_setpoint_s trajectory_setpoint_SL;
typedef struct pollfd pollfd_t;
typedef struct debug_key_value_s debug_key_value_SL;
typedef struct pollfd pollfd_t;
typedef struct landing_target_pose_s landing_target_pose_SL;
typedef struct simulink_motors_outputs_s simulink_motors_outputs_s_t;
typedef struct simulink_servos_outputs_s simulink_servos_outputs_s_t;

/* Block signals for system '<S100>/nan_inf_mode' */
typedef struct {
  ControlMode y;                       /* '<S100>/nan_inf_mode' */
} B_nan_inf_mode_LW_ctrl_V1_11__T;

/* Block signals for system '<S100>/nan_inf_mode1' */
typedef struct {
  FlightMode y;                        /* '<S100>/nan_inf_mode1' */
} B_nan_inf_mode1_LW_ctrl_V1_11_T;

/* Block signals for system '<S93>/nan_inf0' */
typedef struct {
  real32_T y[8];                       /* '<S93>/nan_inf0' */
} B_nan_inf0_LW_ctrl_V1_11_2608_T;

/* Block signals for system '<S94>/hrt_timestamp' */
typedef struct {
  real_T timestamp_us;                 /* '<S94>/hrt_timestamp' */
  real32_T dt;                         /* '<S94>/hrt_timestamp' */
} B_hrt_timestamp_LW_ctrl_V1_11_T;

/* Block states (default storage) for system '<S94>/hrt_timestamp' */
typedef struct {
  real_T timestamp_last_us;            /* '<S94>/hrt_timestamp' */
  boolean_T timestamp_last_us_not_empty;/* '<S94>/hrt_timestamp' */
} DW_hrt_timestamp_LW_ctrl_V1_1_T;

/* Block signals for system '<S114>/nan_inf0' */
typedef struct {
  real32_T y;                          /* '<S114>/nan_inf0' */
} B_nan_inf0_LW_ctrl_V1_11_26_g_T;

/* Block signals for system '<S114>/pitchrate_deadzone1' */
typedef struct {
  real32_T y;                          /* '<S114>/pitchrate_deadzone1' */
} B_pitchrate_deadzone1_LW_ctrl_T;

/* Block signals for system '<S156>/hrt_timestamp' */
typedef struct {
  real_T ts;                           /* '<S156>/hrt_timestamp' */
} B_hrt_timestamp_LW_ctrl_V1__f_T;

/* Block states (default storage) for system '<S156>/hrt_timestamp' */
typedef struct {
  real_T count;                        /* '<S156>/hrt_timestamp' */
} DW_hrt_timestamp_LW_ctrl_V1_c_T;

/* Block signals for system '<S160>/nan_inf' */
typedef struct {
  real32_T y[3];                       /* '<S160>/nan_inf' */
} B_nan_inf_LW_ctrl_V1_11_26081_T;

/* Block signals (default storage) */
typedef struct {
  c_optim_coder_warmstart_Quadp_T ws;
  sTTULnc01OVJqZqpgkzM1HG_LW_ct_T workingset;
  real_T B[45];
  real_T B_r[24];
  vehicle_local_position_SL uORBReadFunctionCallTrigger2;
                                 /* '<S160>/uORB Read Function-Call Trigger2' */
  battery_status_SL uORBReadFunctionCallTrigger6;
                                 /* '<S160>/uORB Read Function-Call Trigger6' */
  lw_mixer_SL uORBReadFunctionCallTrigger7;
                                 /* '<S160>/uORB Read Function-Call Trigger7' */
  sSOeZ9WO10chPn9Si6PKiCB_LW_ct_T options;
  sensor_gps_SL uORBReadFunctionCallTrigger5;
                                 /* '<S160>/uORB Read Function-Call Trigger5' */
  vehicle_imu_status_SL uORBReadFunctionCallTrigger10;
                                /* '<S160>/uORB Read Function-Call Trigger10' */
  rc_channels_SL uORBReadFunctionCallTrigger1;
                                 /* '<S149>/uORB Read Function-Call Trigger1' */
  real_T Wu[16];
  real_T V[16];
  real_T U[16];
  real_T B_r_m[16];
  real_T b_A[16];
  real_T Vf[16];
  creal32_T eigVec[16];
  vehicle_status_SL uORBReadFunctionCallTrigger11;
                                /* '<S160>/uORB Read Function-Call Trigger11' */
  manual_control_setpoint_SL uORBReadFunctionCallTrigger11_l;
                                /* '<S149>/uORB Read Function-Call Trigger11' */
  real32_T candR[18];
  real32_T candR_c[18];
  real_T dv[9];
  real_T inv_uavJ[9];
  real_T work[9];
  real_T V_k[9];
  real_T U_c[9];
  real_T b_A_b[9];
  real_T Vf_p[9];
  real_T work_c[9];
  real_T vn1[9];
  real_T vn2[9];
  real_T b_A_f[9];
  real_T Va[9];
  trajectory_setpoint_SL uORBReadFunctionCallTrigger13;
                                /* '<S160>/uORB Read Function-Call Trigger13' */
  landing_target_pose_SL uORBReadFunctionCallTrigger12;
                                /* '<S160>/uORB Read Function-Call Trigger12' */
  real32_T T[16];
  real32_T R[16];
  real32_T b_A_g[16];
  real32_T Q[16];
  real32_T b_A_g1[16];
  real32_T vr[16];
  real_T Motors_T_m[8];                /* '<S92>/IntegratedMixer' */
  real_T Trj_acc_ppx[8];
  real_T Trj_acc_ppy[8];
  real_T Trj_deacc_ppx[8];
  real_T Trj_deacc_ppy[8];
  vehicle_attitude_SL uORBReadFunctionCallTrigger;
                                  /* '<S160>/uORB Read Function-Call Trigger' */
  airspeed_estimator_SL uORBReadFunctionCallTrigger17;
                                /* '<S160>/uORB Read Function-Call Trigger17' */
  real_T DataTypeConversion_n[7];      /* '<S92>/Data Type Conversion' */
  sensor_combined_SL uORBReadFunctionCallTrigger8;
                                 /* '<S160>/uORB Read Function-Call Trigger8' */
  real32_T B_delta[12];
  real32_T work_p[12];
  vehicle_angular_velocity_SL uORBReadFunctionCallTrigger1_c;
                                 /* '<S160>/uORB Read Function-Call Trigger1' */
  real_T work_l[5];
  real32_T inv_uavJ_j[9];
  real32_T b_b[9];
  real32_T R_eb[9];
  real32_T fv[9];
  real32_T Cd0[9];
  real32_T R_low[9];
  real32_T R_branch_ref[9];
  real32_T R_keep[9];
  real32_T Ri[9];
  real32_T a[9];
  real32_T xw[9];
  real32_T sk[9];
  real32_T Rbe_psi[9];
  real32_T pos_ref_tmp[9];
  real32_T R_low_d[9];
  real32_T R_branch_ref_g[9];
  real32_T xb[9];
  real32_T Ri_l[9];
  real32_T a_d[9];
  real32_T xw_d[9];
  real32_T sk_l[9];
  real32_T Dk[9];
  real32_T b_A_o[9];
  real32_T R_keep_b[9];
  real32_T b_A_n[9];
  sKv95S4FhAbO2Chqql86NVD_LW_ct_T a__1;
  real_T umin[4];
  real_T umax[4];
  real_T q_bl[4];
  real_T dv1[4];
  real_T b_s[4];
  real_T e[4];
  real_T work_b[4];
  real32_T Conversion1[8];             /* '<S92>/Conversion1' */
  real32_T Conversion5[8];             /* '<S92>/Conversion5' */
  creal32_T eigVal[4];
  real32_T H[6];
  real32_T K_raw[6];
  real32_T K_raw_l[6];
  real32_T work_h[6];
  real_T Fa_body[3];
  real_T s[3];
  real_T b_s_b[3];
  real_T e_d[3];
  real_T work_e[3];
  real_T x[3];
  sL9bDKomAYkxZSVrG9w6En_LW_ctr_T runTimeOptions;
  takeoff_status_SL uORBReadFunctionCallTrigger15;
                                /* '<S160>/uORB Read Function-Call Trigger15' */
  debug_key_value_SL uORBReadFunctionCallTrigger14;
                                /* '<S160>/uORB Read Function-Call Trigger14' */
  actuator_armed_SL uORBReadFunctionCallTrigger9;
                                 /* '<S160>/uORB Read Function-Call Trigger9' */
  real32_T q[4];
  real32_T q_inv[4];
  real32_T q_temp[4];
  real32_T q_err[4];
  real32_T q_el[4];
  real32_T x_b[4];
  real32_T a__4[4];
  real32_T work_j[4];
  real32_T wr[4];
  real32_T wi[4];
  real32_T scale[4];
  real32_T x_f[4];
  real32_T cr[4];
  real32_T ci[4];
  real32_T work_a[4];
  real32_T q_inv_j[4];
  control_flag_bus ctrl_flag;          /* '<S7>/MATLAB Function4' */
  boolean_T bv[16];
  int96m_T r;
  int96m_T r1;
  int96m_T r2;
  real32_T v_wind[3];                  /* '<S160>/Switch1' */
  real32_T vehicle_euler[3];
  real32_T beta2[3];
  real32_T e_omega[3];
  real32_T c_b[3];
  real32_T u_j[3];
  real32_T f_dist[3];
  real32_T TmpSignalConversionAtSFun_g[3];/* '<S21>/attrates_ref_selector' */
  real32_T TmpSignalConversionAtDotPro[3];
  real32_T land_detector_vel_ref[3];
  real32_T B_r_o[3];
  real32_T tau[3];
  real32_T e_n[3];
  real32_T tau_i[3];
  real32_T va_ref[3];
  real32_T an[3];
  real32_T xc[3];
  real32_T y_perp[3];
  real32_T xw_low_mem[3];
  real32_T yb_low_mem[3];
  real32_T zw_low_mem[3];
  real32_T zb_low[3];
  real32_T r_o[3];
  real32_T c[3];
  real32_T b_xw[3];
  real32_T xb_h[3];
  real32_T yb[3];
  real32_T zb[3];
  real32_T va_ref_n[3];
  real32_T an_m[3];
  real32_T xc_c[3];
  real32_T y_perp_m[3];
  real32_T xw_low_mem_m[3];
  real32_T zw_low_mem_j[3];
  real32_T xc0[3];
  real32_T yc0[3];
  real32_T r_h[3];
  real32_T zb_c[3];
  real32_T xb_c[3];
  real32_T r_p[3];
  real32_T xb_h_p[3];
  real32_T v[3];
  char_T expl_temp[10];
  boolean_T x_a[9];
  boolean_T bv1[9];
  boolean_T x_e[9];
  boolean_T bv2[9];
  real_T timestamp_us;                 /* '<S10>/hrt_timestamp' */
  real_T hrt;
  real_T T_hover;
  real_T absx;
  real_T pooled1;
  real_T d;
  real_T B_r_a;
  real_T B_r_tmp;
  real_T H_infnrm;
  real_T f_infnrm;
  real_T colSum;
  real_T tolDelta;
  real_T normDelta;
  real_T s_a;
  real_T temp;
  real_T temp_tmp;
  real_T constrViolation_basicX;
  real_T c_i;
  real_T b_atmp;
  real_T tau_l;
  real_T xnorm;
  real_T a_o;
  real_T scale_o;
  real_T absxk;
  real_T t;
  real_T x_i;
  real_T absx_f;
  real_T nrm;
  real_T rt;
  real_T ztest;
  real_T smm1;
  real_T emm1;
  real_T sqds;
  real_T shift;
  real_T roe;
  real_T absa;
  real_T absb;
  real_T tmp;
  real_T vx;
  real_T vy;
  real_T d1;
  real_T d2;
  real_T d3;
  real_T d4;
  real_T d5;
  real_T y_i;
  real_T nrm_f;
  real_T rt_g;
  real_T ztest_c;
  real_T smm1_o;
  real_T emm1_l;
  real_T tol;
  real_T qtb;
  real_T tol_m;
  real_T temp_m;
  real_T s_c;
  real_T vn1_f;
  real_T c_p;
  real_T xd;
  real_T smax;
  real_T s_e;
  real_T temp_o;
  real_T alpha1;
  real_T temp_h;
  real_T p_max;
  real_T denomTol;
  real_T phaseOneCorrectionX;
  real_T phaseOneCorrectionP;
  real_T denomTol_l;
  real_T phaseOneCorrectionX_h;
  real_T c_c;
  real_T b_s_m;
  real_T a_m;
  real_T scale_h;
  real_T y_c;
  real32_T wind_alg[2];
  real32_T wind_step[2];
  real32_T stick_input_xy[2];
  real32_T vehicle_euler_k[2];
  real32_T vehicle_euler_p[2];
  real32_T candAlpha[2];
  real32_T candF[2];
  real32_T candAttCost[2];
  real32_T candAlphaCost[2];
  real32_T candYawCost[2];
  real32_T candAlpha_p[2];
  real32_T candF_p[2];
  real32_T acc_target_body_raw[2];
  real32_T acc_stick_ned[2];
  real32_T drag_data[2];
  real32_T vel_sp_body[2];
  real32_T vel_target_body_raw[2];
  uint64m_T dif;
  uint64m_T r3;
  uint64m_T r4;
  uint64m_T r5;
  uint64m_T r6;
  uint64m_T r7;
  int32_T drag_size[2];
  int32_T dir_size[2];
  boolean_T bv3[6];
  real32_T yaw;                        /* '<S160>/Gain' */
  real32_T y[4];                       /* '<S160>/nan_inf_q' */
  real32_T y_g[3];                     /* '<S160>/nan_inf0' */
  real32_T throttle;                   /* '<S149>/Divide' */
  real32_T DataTypeConversion;         /* '<S152>/Data Type Conversion' */
  real32_T y_o[18];                    /* '<S149>/MATLAB Function' */
  real32_T DataTypeConversion_p;       /* '<S157>/Data Type Conversion' */
  real32_T stick_xyzr[4];
                    /* '<S5>/BusConversion_InsertedFor_StatesRef_at_inport_0' */
  real32_T DataTypeConversion5_h;      /* '<S117>/Data Type Conversion5' */
  real32_T pos_abs[3];
             /* '<S115>/BusConversion_InsertedFor_vision_landing_at_inport_0' */
  real32_T yaw_p;
             /* '<S115>/BusConversion_InsertedFor_vision_landing_at_inport_0' */
  real32_T pos_ref[3];                 /* '<S115>/df' */
  real32_T vel_ref[3];                 /* '<S115>/df' */
  real32_T acc_ref[3];                 /* '<S115>/df' */
  real32_T q_ref[4];                   /* '<S115>/df' */
  real32_T f_d;                        /* '<S115>/df' */
  real32_T alpha;                      /* '<S115>/df' */
  real32_T beta;                       /* '<S115>/df' */
  real32_T wb_ref[3];                  /* '<S115>/df' */
  real32_T dotwb_ref[3];               /* '<S115>/df' */
  real32_T vel_ref_j[3];               /* '<S114>/Data Type Conversion3' */
  real32_T delta_r;                    /* '<S114>/Gain1' */
  real32_T delta_a;                    /* '<S114>/Gain3' */
  real32_T thrust;                     /* '<S114>/Gain4' */
  real32_T delta_e;                    /* '<S114>/Gain8' */
  real32_T yaw_ref_n;                  /* '<S114>/Constant' */
  real32_T roll_ref;                   /* '<S114>/deg2rad' */
  real32_T pitch_ref;                  /* '<S114>/deg2rad2' */
  real32_T rollrate_ref;               /* '<S114>/deg2rad4' */
  real32_T pitchRate_d;                /* '<S114>/deg2rad5' */
  real32_T clean_wz_sp;                /* '<S114>/Lowpass3' */
  real32_T Merge[4];                   /* '<S38>/Merge' */
  real32_T y_k[4];                     /* '<S14>/nan_inf_q' */
  real32_T y_g3[3];                    /* '<S14>/nan_inf0' */
  real32_T yaw_ref_j;                  /* '<S90>/MATLAB Function' */
  real32_T pos_ref_l[3];               /* '<S33>/position control' */
  real32_T vd_lim[3];                  /* '<S33>/position control' */
  real32_T man_acc_ff[3];              /* '<S33>/position control' */
  real32_T accel_sp[3];                /* '<S31>/velocity_pid' */
  real32_T tilt_limit;                 /* '<S30>/update_takeoff_status' */
  real32_T d_est[3];                   /* '<S30>/MATLAB Function3' */
  real32_T vk_[3];                     /* '<S30>/DisturbanceFEstimator' */
  real32_T pk_[3];                     /* '<S30>/DisturbanceFEstimator' */
  real32_T ae_bias_hover[3];           /* '<S30>/DisturbanceFEstimator' */
  real32_T v_wind_est[3];              /* '<S30>/AirspeedEstimator' */
  real32_T airspeed_est;               /* '<S30>/AirspeedEstimator' */
  real32_T debug[8];                   /* '<S30>/AirspeedEstimator' */
  real32_T DataTypeConversion4;        /* '<S29>/Data Type Conversion4' */
  real32_T DataTypeConversion5_m;      /* '<S29>/Data Type Conversion5' */
  real32_T DataTypeConversion6_f;      /* '<S29>/Data Type Conversion6' */
  real32_T rates_ref[3];               /* '<S21>/attrates_ref_selector' */
  real32_T u[3];                       /* '<S21>/MATLAB Function' */
  real32_T us;                         /* '<S21>/MATLAB Function' */
  real32_T f_now;                      /* '<S21>/MATLAB Function' */
  real32_T phase;                      /* '<S21>/MATLAB Function' */
  real32_T d_est_g[3];                 /* '<S21>/Lowpass' */
  real32_T omegak_[3];                 /* '<S21>/DisturbanceEstimator' */
  real32_T qk_[4];                     /* '<S21>/DisturbanceEstimator' */
  real32_T q_ref_b[4];                 /* '<S15>/MATLAB Function' */
  real32_T thrust_ref[3];              /* '<S15>/MATLAB Function' */
  real32_T Conversion2;                /* '<S92>/Conversion2' */
  real32_T Conversion3;                /* '<S92>/Conversion3' */
  real32_T Conversion4;                /* '<S92>/Conversion4' */
  real32_T throttle_k[16];             /* '<S93>/Saturation' */
  real32_T Motors_T[8];                /* '<S100>/MATLAB Function' */
  real32_T Servos_rad[8];              /* '<S100>/MATLAB Function' */
  real32_T aSinInput;
  real32_T norm_proj;
  real32_T yaw_err;
  real32_T qm;
  real32_T out_y;
  real32_T out_z;
  real32_T Cd0_a;
  real32_T Cy0;
  real32_T c_aero;
  real32_T wind_step_norm;
  real32_T tracking_gain_x;
  real32_T pi_z;
  real32_T Add;                        /* '<S47>/Add' */
  real32_T q_ref_b_tmp;
  real32_T K12;
  real32_T K13;
  real32_T K14;
  real32_T K23;
  real32_T K24;
  real32_T K34;
  real32_T anrm;
  real32_T absx_j;
  real32_T temp1;
  real32_T temp2;
  real32_T tau_tmp;
  real32_T anrm_e;
  real32_T absxk_o;
  real32_T s_b;
  real32_T gs;
  real32_T b_temp;
  real32_T vr_a;
  real32_T gs_tmp;
  real32_T gs_tmp_g;
  real32_T vr_tmp;
  real32_T gs_tmp_e;
  real32_T vr_tmp_f;
  real32_T wr_h;
  real32_T wi_e;
  real32_T smin;
  real32_T scale_c;
  real32_T rec;
  real32_T u0;
  real32_T cmax;
  real32_T temp_a;
  real32_T ur11r;
  real32_T lr21;
  real32_T ur22;
  real32_T br2;
  real32_T ui11r;
  real32_T li21;
  real32_T Va_ref;
  real32_T qS;
  real32_T v_switch;
  real32_T v_low;
  real32_T f_low;
  real32_T cn;
  real32_T e_nv;
  real32_T yb_low_mem_d;
  real32_T yb_low_mem_a;
  real32_T cn_p;
  real32_T xb_idx_1;
  real32_T xb_idx_0;
  real32_T xb_idx_2;
  real32_T sk_m;
  real32_T H_o;
  real32_T Aalpha;
  real32_T bestCost;
  real32_T thrustViolation;
  real32_T yawViolation;
  real32_T Va_ref_n;
  real32_T qS_l;
  real32_T v_switch_p;
  real32_T v_low_p;
  real32_T f_low_f;
  real32_T smlnum;
  real32_T tst;
  real32_T bb;
  real32_T s_i;
  real32_T h12;
  real32_T h21;
  real32_T h22;
  real32_T tst_o;
  real32_T p;
  real32_T alpha1_k;
  real32_T cn_i;
  real32_T e_nv_o;
  real32_T xc0_m;
  real32_T xc0_c;
  real32_T c_f;
  real32_T r_hc;
  int32_T r_m;
  int32_T ar;
  int32_T b_i;
  int32_T aoffset;
  int32_T g;
  int32_T vcol;
  int32_T i;
  int32_T tmp_size;
  int32_T mAll;
  int32_T currentMplier;
  int32_T idxOffset;
  int32_T PROBTYPE_ORIG;
  int32_T nVar;
  int32_T mConstr;
  int32_T idxStartIneq;
  int32_T idxEndIneq;
  int32_T idxStartIneq_tmp;
  int32_T activeSetChangeID;
  int32_T nVar_a;
  int32_T globalActiveConstrIdx;
  int32_T TYPE;
  int32_T idxRotGCol;
  int32_T Qk0;
  int32_T iy;
  int32_T iyend;
  int32_T b_iy;
  int32_T l;
  int32_T mWConstr;
  int32_T nVar_k;
  int32_T idx;
  int32_T k;
  int32_T b_k;
  int32_T c_j;
  int32_T sgn;
  int32_T ii;
  int32_T b_ix;
  int32_T b_iy_p;
  int32_T tau_tmp_tmp;
  int32_T sgn_tmp;
  int32_T b_iy_tmp;
  int32_T e_tmp_tmp;
  int32_T ai_tmp;
  int32_T k_b;
  int32_T ihi;
  int32_T info;
  int32_T b_k_c;
  int32_T c_ix;
  int32_T b_temp_tmp;
  int32_T b_temp_tmp_tmp;
  int32_T b_temp_tmp_tmp_n;
  int32_T j;
  int32_T ip;
  int32_T b_j;
  int32_T iyend_i;
  int32_T b_ix_m;
  int32_T d_j;
  int32_T T_tmp;
  int32_T T_tmp_tmp;
  int32_T T_tmp_e;
  int32_T icmax;
  int32_T k_m;
  int32_T i_m;
  int32_T bestIdx;
  int32_T k_j;
  int32_T mWorkingFixed;
  int32_T c_fo;
  int32_T totalRank;
  int32_T ix;
  int32_T u1;
  int32_T mTotalWorkingEq_tmp;
  int32_T c_tmp_tmp;
  int32_T nActiveConstr;
  int32_T nFixedConstr;
  int32_T nVar_a5;
  int32_T nDepIneq;
  int32_T c_g;
  int32_T iy0_tmp;
  int32_T nfxd;
  int32_T j_n;
  int32_T i_d;
  int32_T nz;
  int32_T kdefl;
  int32_T l_n;
  int32_T k_c;
  int32_T m;
  int32_T nr;
  int32_T l1;
  int32_T m_f;
  int32_T l_p;
  int32_T lsv;
  int32_T lend;
  int32_T lendsv;
  int32_T iscale;
  int32_T nVar_p;
  int32_T im1n;
  int32_T i_n;
  int32_T lastv;
  int32_T lastc;
  int32_T i1;
  int32_T ica;
  int32_T ix_k;
  uint32_T DataTypeConversion10;       /* '<S151>/Data Type Conversion10' */
  uint32_T DataTypeConversion5;        /* '<S151>/Data Type Conversion5' */
  uint32_T DataTypeConversion6;        /* '<S151>/Data Type Conversion6' */
  uint32_T DataTypeConversion7;        /* '<S151>/Data Type Conversion7' */
  uint32_T DataTypeConversion9;        /* '<S151>/Data Type Conversion9' */
  real32_T true_airspeed_m_s;          /* '<S160>/Switch' */
  FlightMode flight_mode;              /* '<S7>/MATLAB Function4' */
  ControlMode ctrl_mode;               /* '<S7>/MATLAB Function4' */
  ControlMode arm;                     /* '<S7>/MATLAB Function4' */
  ControlMode y_n;                     /* '<S93>/nan_inf_mode' */
  FlightMode flight_mode_n;
  boolean_T bv4[4];
  uint8_T DataTypeConversion1;         /* '<S156>/Data Type Conversion1' */
  uint8_T DataTypeConversion10_o;      /* '<S156>/Data Type Conversion10' */
  uint8_T DataTypeConversion11;        /* '<S156>/Data Type Conversion11' */
  uint8_T DataTypeConversion12;        /* '<S156>/Data Type Conversion12' */
  uint8_T DataTypeConversion13;        /* '<S156>/Data Type Conversion13' */
  uint8_T DataTypeConversion2;         /* '<S156>/Data Type Conversion2' */
  uint8_T DataTypeConversion3;         /* '<S156>/Data Type Conversion3' */
  uint8_T DataTypeConversion4_f;       /* '<S156>/Data Type Conversion4' */
  uint8_T DataTypeConversion5_p;       /* '<S156>/Data Type Conversion5' */
  uint8_T DataTypeConversion6_p;       /* '<S156>/Data Type Conversion6' */
  uint8_T DataTypeConversion7_g;       /* '<S156>/Data Type Conversion7' */
  uint8_T DataTypeConversion8;         /* '<S156>/Data Type Conversion8' */
  uint8_T DataTypeConversion9_j;       /* '<S156>/Data Type Conversion9' */
  uint8_T auto_landing_detector_state; /* '<S33>/position control' */
  uint8_T takeoff_state;               /* '<S30>/update_takeoff_status' */
  int8_T OB_TASK;                      /* '<S115>/df' */
  boolean_T Compare;                   /* '<S11>/Compare' */
  boolean_T abs_pos_valid;
             /* '<S115>/BusConversion_InsertedFor_vision_landing_at_inport_0' */
  boolean_T integral_reset[2];         /* '<S33>/position control' */
  boolean_T auto_landing_detector_debug[7];/* '<S33>/position control' */
  boolean_T freefall;                  /* '<S30>/LandDetector' */
  boolean_T ground_contact;            /* '<S30>/LandDetector' */
  boolean_T maybe_landed;              /* '<S30>/LandDetector' */
  boolean_T landed;                    /* '<S30>/LandDetector' */
  boolean_T in_descend;                /* '<S30>/LandDetector' */
  boolean_T vertical_movement;         /* '<S30>/LandDetector' */
  boolean_T horizontal_movement;       /* '<S30>/LandDetector' */
  boolean_T has_low_throttle;          /* '<S30>/LandDetector' */
  boolean_T below_gnd_effect;          /* '<S30>/LandDetector' */
  boolean_T close_or_skipped;          /* '<S30>/LandDetector' */
  boolean_T rotational_movement;       /* '<S30>/LandDetector' */
  boolean_T at_rest_final;             /* '<S30>/LandDetector' */
  B_nan_inf_LW_ctrl_V1_11_26081_T sf_nan_inf1_f;/* '<S160>/nan_inf1' */
  B_nan_inf_LW_ctrl_V1_11_26081_T sf_nan_inf;/* '<S160>/nan_inf' */
  B_hrt_timestamp_LW_ctrl_V1__f_T sf_hrt_timestamp_knt;/* '<S151>/hrt_timestamp' */
  B_hrt_timestamp_LW_ctrl_V1__f_T sf_hrt_timestamp_p;/* '<S156>/hrt_timestamp' */
  B_pitchrate_deadzone1_LW_ctrl_T sf_yawrate_deadzone1;/* '<S114>/yawrate_deadzone1' */
  B_pitchrate_deadzone1_LW_ctrl_T sf_rollrate_deadzone1;/* '<S114>/rollrate_deadzone1' */
  B_pitchrate_deadzone1_LW_ctrl_T sf_pitchrate_deadzone1;/* '<S114>/pitchrate_deadzone1' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf9;/* '<S114>/nan_inf9' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf8;/* '<S114>/nan_inf8' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf7;/* '<S114>/nan_inf7' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf6;/* '<S114>/nan_inf6' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf5;/* '<S114>/nan_inf5' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf4;/* '<S114>/nan_inf4' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf3;/* '<S114>/nan_inf3' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf2;/* '<S114>/nan_inf2' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf11;/* '<S114>/nan_inf11' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf10;/* '<S114>/nan_inf10' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf1_c;/* '<S114>/nan_inf1' */
  B_nan_inf0_LW_ctrl_V1_11_26_g_T sf_nan_inf0_m;/* '<S114>/nan_inf0' */
  B_hrt_timestamp_LW_ctrl_V1_11_T sf_hrt_timestamp_kn;/* '<S112>/hrt_timestamp' */
  B_nan_inf_mode1_LW_ctrl_V1_11_T sf_nan_inf_mode1_g;/* '<S33>/nan_inf_mode1' */
  B_nan_inf_mode_LW_ctrl_V1_11__T sf_nan_inf_mode_c;/* '<S33>/nan_inf_mode' */
  B_hrt_timestamp_LW_ctrl_V1_11_T sf_hrt_timestamp_k;/* '<S32>/hrt_timestamp' */
  B_hrt_timestamp_LW_ctrl_V1_11_T sf_hrt_timestamp_a;/* '<S17>/hrt_timestamp' */
  B_hrt_timestamp_LW_ctrl_V1_11_T sf_hrt_timestamp_d;/* '<S94>/hrt_timestamp' */
  B_nan_inf0_LW_ctrl_V1_11_2608_T sf_nan_inf1;/* '<S93>/nan_inf1' */
  B_nan_inf0_LW_ctrl_V1_11_2608_T sf_nan_inf0;/* '<S93>/nan_inf0' */
  B_nan_inf_mode1_LW_ctrl_V1_11_T sf_nan_inf_mode1;/* '<S100>/nan_inf_mode1' */
  B_nan_inf_mode_LW_ctrl_V1_11__T sf_nan_inf_mode;/* '<S100>/nan_inf_mode' */
} B_LW_ctrl_V1_11_260814_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  debug_array_s_t uORBWriteAdvanced1_uorb_msg;/* '<S29>/uORB Write Advanced1' */
  lw_mixer_s_t uORBWriteAdvanced_uorb_msg;/* '<S111>/uORB Write Advanced' */
  lw_navigator_s_t uORBWriteAdvanced_uorb_msg_h;/* '<S117>/uORB Write Advanced' */
  lw_posctrl_s_t uORBWriteAdvanced_uorb_msg_m;/* '<S90>/uORB Write Advanced' */
  lw_attctrl_s_t uORBWriteAdvanced_uorb_msg_k;/* '<S29>/uORB Write Advanced' */
  LowPassFilter2pVector3_LW_ctr_T lpf_omegam;/* '<S21>/DisturbanceEstimator' */
  TrajectoryGeneratorXY_LW_ctrl_T traj_gen_xy;/* '<S33>/position control' */
  liftwingFlatnessSolver_LW_c_e_T solver;/* '<S115>/df' */
  liftwingFlatnessSolver_LW_ctr_T flatSolver;/* '<S30>/MATLAB Function1' */
  vehicle_local_position_setpoint_s_t uORBWriteAdvanced3_uorb_msg;/* '<S90>/uORB Write Advanced3' */
  airspeed_estimator_s_t uORBWriteAdvanced5_uorb_msg;/* '<S90>/uORB Write Advanced5' */
  lw_manualctrl_s_t uORBWriteAdvanced_uorb_msg_d;/* '<S151>/uORB Write Advanced' */
  simulink_motors_outputs_s_t uORBWriteAdvanced_uorb_msg_a;/* '<S10>/uORB Write Advanced' */
  simulink_servos_outputs_s_t uORBWriteAdvanced1_uorb_msg_i;/* '<S10>/uORB Write Advanced1' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_x;/* '<S21>/rates_control_to_torque' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_y;/* '<S21>/rates_control_to_torque' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_z;/* '<S21>/rates_control_to_torque' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_x_k;/* '<S31>/velocity_pid' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_y_e;/* '<S31>/velocity_pid' */
  RflyDiscretePID_LW_ctrl_V1_11_T pid_z_o;/* '<S31>/velocity_pid' */
  vision_landing_s_t uORBWriteAdvanced1_uorb_msg_h;/* '<S117>/uORB Write Advanced1' */
  lw_statemachine_s_t uORBWriteAdvanced1_uorb_msg_p;/* '<S156>/uORB Write Advanced1' */
  vehicle_land_detected_s_t uORBWriteAdvanced2_uorb_msg;/* '<S90>/uORB Write Advanced2' */
  auto_landing_detector_s_t uORBWriteAdvanced4_uorb_msg;/* '<S90>/uORB Write Advanced4' */
  takeoff_status_s_t uORBWriteAdvanced1_uorb_msg_d;/* '<S90>/uORB Write Advanced1' */
  pollfd_t uORBReadFunctionCallTrigger2_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger2' */
  pollfd_t uORBReadFunctionCallTrigger_uOR;
                                  /* '<S160>/uORB Read Function-Call Trigger' */
  pollfd_t uORBReadFunctionCallTrigger17_u;
                                /* '<S160>/uORB Read Function-Call Trigger17' */
  pollfd_t uORBReadFunctionCallTrigger1_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger1' */
  pollfd_t uORBReadFunctionCallTrigger5_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger5' */
  pollfd_t uORBReadFunctionCallTrigger6_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger6' */
  pollfd_t uORBReadFunctionCallTrigger7_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger7' */
  pollfd_t uORBReadFunctionCallTrigger10_u;
                                /* '<S160>/uORB Read Function-Call Trigger10' */
  pollfd_t uORBReadFunctionCallTrigger8_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger8' */
  pollfd_t uORBReadFunctionCallTrigger11_u;
                                /* '<S160>/uORB Read Function-Call Trigger11' */
  pollfd_t uORBReadFunctionCallTrigger15_u;
                                /* '<S160>/uORB Read Function-Call Trigger15' */
  pollfd_t uORBReadFunctionCallTrigger9_uO;
                                 /* '<S160>/uORB Read Function-Call Trigger9' */
  pollfd_t uORBReadFunctionCallTrigger13_u;
                                /* '<S160>/uORB Read Function-Call Trigger13' */
  pollfd_t uORBReadFunctionCallTrigger14_u;
                                /* '<S160>/uORB Read Function-Call Trigger14' */
  pollfd_t uORBReadFunctionCallTrigger12_u;
                                /* '<S160>/uORB Read Function-Call Trigger12' */
  pollfd_t ParamUpdate_uORB_fd;        /* '<S160>/ParamUpdate' */
  pollfd_t uORBReadFunctionCallTrigger11_b;
                                /* '<S149>/uORB Read Function-Call Trigger11' */
  pollfd_t uORBReadFunctionCallTrigger1__f;
                                 /* '<S149>/uORB Read Function-Call Trigger1' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_x;/* '<S21>/Lowpass' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_y;/* '<S21>/Lowpass' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_z;/* '<S21>/Lowpass' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_alpha_des_x;/* '<S21>/rates_control_to_torque' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_alpha_des_y;/* '<S21>/rates_control_to_torque' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_alpha_des_z;/* '<S21>/rates_control_to_torque' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_x_c;/* '<S30>/MATLAB Function3' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_y_a;/* '<S30>/MATLAB Function3' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_d_est_z_d;/* '<S30>/MATLAB Function3' */
  LowPassFilter1p_LW_ctrl_V1_11_T vel_dot_filter_x;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T vel_dot_filter_y;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T vel_dot_filter_z;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_acc_x;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_acc_y;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_acc_z;/* '<S31>/velocity_pid' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_stick_x;/* '<S33>/position control' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_stick_y;/* '<S33>/position control' */
  emxArray_real32_T_1x1_LW_ctrl_T filter_state;/* '<S33>/position control' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_vel_sp;/* '<S114>/Lowpass' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_vel_sp_g;/* '<S114>/Lowpass1' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_vel_sp_gy;/* '<S114>/Lowpass2' */
  LowPassFilter1p_LW_ctrl_V1_11_T lpf_wz_sp;/* '<S114>/Lowpass3' */
  real_T Memory_PreviousInput;         /* '<S152>/Memory' */
  real_T Memory_PreviousInput_f;       /* '<S157>/Memory' */
  real_T hold_z_timestamp;             /* '<S33>/position control' */
  real_T hys_last_time_change_us;      /* '<S30>/update_takeoff_status' */
  real_T time_last_move_us;            /* '<S30>/LandDetector' */
  real_T delta_l[4];                   /* '<S92>/IntegratedMixer' */
  real_T relaxed_yaw_;                 /* '<S92>/IntegratedMixer' */
  real32_T Delay5_DSTATE[3];           /* '<S33>/Delay5' */
  real32_T Delay6_DSTATE[3];           /* '<S33>/Delay6' */
  real32_T Delay2_DSTATE;              /* '<S33>/Delay2' */
  orb_advert_t uORBWriteAdvanced_uorb_advert;/* '<S10>/uORB Write Advanced' */
  orb_advert_t uORBWriteAdvanced1_uorb_advert;/* '<S10>/uORB Write Advanced1' */
  orb_advert_t uORBWriteAdvanced_uorb_advert_k;/* '<S151>/uORB Write Advanced' */
  orb_advert_t uORBWriteAdvanced1_uorb_adver_o;/* '<S156>/uORB Write Advanced1' */
  orb_advert_t uORBWriteAdvanced_uorb_advert_p;/* '<S117>/uORB Write Advanced' */
  orb_advert_t uORBWriteAdvanced1_uorb_adver_c;/* '<S117>/uORB Write Advanced1' */
  orb_advert_t uORBWriteAdvanced1_uorb_adver_f;/* '<S90>/uORB Write Advanced1' */
  orb_advert_t uORBWriteAdvanced2_uorb_advert;/* '<S90>/uORB Write Advanced2' */
  orb_advert_t uORBWriteAdvanced4_uorb_advert;/* '<S90>/uORB Write Advanced4' */
  orb_advert_t uORBWriteAdvanced5_uorb_advert;/* '<S90>/uORB Write Advanced5' */
  orb_advert_t uORBWriteAdvanced_uorb_advert_n;/* '<S90>/uORB Write Advanced' */
  orb_advert_t uORBWriteAdvanced3_uorb_advert;/* '<S90>/uORB Write Advanced3' */
  orb_advert_t uORBWriteAdvanced_uorb_advert_f;/* '<S29>/uORB Write Advanced' */
  orb_advert_t uORBWriteAdvanced1_uorb_adver_l;/* '<S29>/uORB Write Advanced1' */
  orb_advert_t uORBWriteAdvanced_uorb_adver_f1;/* '<S111>/uORB Write Advanced' */
  real32_T time_start;                 /* '<S115>/MissionSwtich' */
  real32_T pos_last[3];                /* '<S115>/MissionSwtich' */
  real32_T yaw_last;                   /* '<S115>/MissionSwtich' */
  real32_T pos_start[3];               /* '<S115>/MissionSwtich' */
  real32_T pos_deacc_start[3];         /* '<S115>/MissionSwtich' */
  real32_T Acct;                       /* '<S115>/MissionSwtich' */
  real32_T Unit;                       /* '<S115>/MissionSwtich' */
  real32_T takeoff_yaw;                /* '<S115>/MissionSwtich' */
  real32_T last_ref[3];                /* '<S115>/MissionSwtich' */
  real32_T last_time;                  /* '<S115>/MissionSwtich' */
  real32_T last_abs_pos[3];            /* '<S115>/MissionSwtich' */
  real32_T last_pos[3];                /* '<S115>/MissionSwtich' */
  real32_T hold_yaw_ref;               /* '<S115>/MissionSwtich' */
  real32_T pos_z1;                     /* '<S33>/position control' */
  real32_T yaw_d_;                     /* '<S33>/position control' */
  real32_T hold_vz;                    /* '<S33>/position control' */
  real32_T hold_pz;                    /* '<S33>/position control' */
  real32_T roll_ref;                   /* '<S33>/position control' */
  real32_T time_in_land;               /* '<S33>/position control' */
  real32_T last_z_ref;                 /* '<S33>/position control' */
  real32_T last_z;                     /* '<S33>/position control' */
  real32_T cmd_down_win;               /* '<S33>/position control' */
  real32_T actual_down_win;            /* '<S33>/position control' */
  real32_T cmd_up_win;                 /* '<S33>/position control' */
  real32_T vz_lpf;                     /* '<S33>/position control' */
  real32_T last_vz_ref;                /* '<S33>/position control' */
  real32_T command_stable_timer;       /* '<S33>/position control' */
  real32_T descent_intent_timer;       /* '<S33>/position control' */
  real32_T descent_motion_timer;       /* '<S33>/position control' */
  real32_T descent_recent_timer;       /* '<S33>/position control' */
  real32_T support_lpf;                /* '<S33>/position control' */
  real32_T support_baseline;           /* '<S33>/position control' */
  real32_T candidate_timer;            /* '<S33>/position control' */
  real32_T blocked_timer;              /* '<S33>/position control' */
  real32_T support_timer;              /* '<S33>/position control' */
  real32_T accel_state[3];             /* '<S31>/velocity_pid' */
  real32_T pi_tracking_prev[3];        /* '<S31>/velocity_pid' */
  real32_T vel_previous[3];            /* '<S31>/velocity_pid' */
  real32_T ramp_progress;              /* '<S30>/update_takeoff_status' */
  real32_T ramp_vz_init;               /* '<S30>/update_takeoff_status' */
  real32_T tilt_limit_rad;             /* '<S30>/update_takeoff_status' */
  real32_T xw_hist[3];                 /* '<S30>/MATLAB Function1' */
  real32_T yb_hist[3];                 /* '<S30>/MATLAB Function1' */
  real32_T zb_hist[3];                 /* '<S30>/MATLAB Function1' */
  real32_T u_v_hist[3];                /* '<S30>/MATLAB Function1' */
  real32_T timer_freefall;             /* '<S30>/LandDetector' */
  real32_T timer_contact;              /* '<S30>/LandDetector' */
  real32_T timer_maybe;                /* '<S30>/LandDetector' */
  real32_T timer_landed;               /* '<S30>/LandDetector' */
  real32_T pk[3];                      /* '<S30>/DisturbanceFEstimator' */
  real32_T vk[3];                      /* '<S30>/DisturbanceFEstimator' */
  real32_T z3[3];                      /* '<S30>/DisturbanceFEstimator' */
  real32_T T_filtered_state[16];       /* '<S30>/DisturbanceFEstimator' */
  real32_T ae_bias_hover_state[3];     /* '<S30>/DisturbanceFEstimator' */
  real32_T wind_h[2];                  /* '<S30>/AirspeedEstimator' */
  real32_T P_w[4];                     /* '<S30>/AirspeedEstimator' */
  real32_T T_filtered_state_i[16];     /* '<S30>/AirspeedEstimator' */
  real32_T wind_fuse_weight;           /* '<S30>/AirspeedEstimator' */
  real32_T alpha_hat[3];               /* '<S21>/rates_control_to_torque' */
  real32_T alpha_c_last[3];            /* '<S21>/rates_control_to_torque' */
  real32_T alpha_des_f_last[3];        /* '<S21>/rates_control_to_torque' */
  real32_T alpha_des_cutoff_xy_last;   /* '<S21>/rates_control_to_torque' */
  real32_T alpha_des_cutoff_z_last;    /* '<S21>/rates_control_to_torque' */
  real32_T sample_time_alpha_lpf_last; /* '<S21>/rates_control_to_torque' */
  real32_T t_start;                    /* '<S21>/MATLAB Function' */
  real32_T phase_last;                 /* '<S21>/MATLAB Function' */
  real32_T t_last;                     /* '<S21>/MATLAB Function' */
  real32_T tau_omegak[3];              /* '<S21>/DisturbanceEstimator' */
  real32_T tau_z2[3];                  /* '<S21>/DisturbanceEstimator' */
  real32_T T_filtered_state_m[16];     /* '<S21>/DisturbanceEstimator' */
  real32_T altitude_yaw_d_;            /* '<S15>/MATLAB Function' */
  int32_T last_traj_flag;              /* '<S115>/df' */
  int32_T valid_cnt;                   /* '<S115>/MissionSwtich' */
  int32_T lw_pos_mode_;                /* '<S33>/position control' */
  uint32_T temporalCounter_i1;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i2;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i3;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i4;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i5;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i6;         /* '<Root>/Scheduler' */
  uint32_T temporalCounter_i7;         /* '<Root>/Scheduler' */
  uint8_T Delay2_DSTATE_a;             /* '<S30>/Delay2' */
  uint8_T Delay1_DSTATE_n;             /* '<S30>/Delay1' */
  uint8_T Delay_DSTATE_b;              /* '<S33>/Delay' */
  boolean_T Delay3_DSTATE;             /* '<S33>/Delay3' */
  boolean_T Delay4_DSTATE;             /* '<S33>/Delay4' */
  int8_T lineMode;                     /* '<S115>/MissionSwtich' */
  uint8_T is_active_c3_LW_ctrl_V1_11_2608;/* '<Root>/Scheduler' */
  uint8_T xy_reset_counter_;           /* '<S115>/MissionSwtich' */
  uint8_T z_reset_counter_;            /* '<S115>/MissionSwtich' */
  uint8_T heading_reset_counter_;      /* '<S115>/MissionSwtich' */
  uint8_T xy_reset_counter__p;         /* '<S33>/position control' */
  uint8_T z_reset_counter__m;          /* '<S33>/position control' */
  uint8_T vxy_reset_counter_;          /* '<S33>/position control' */
  uint8_T vz_reset_counter_;           /* '<S33>/position control' */
  uint8_T heading_reset_counter__k;    /* '<S33>/position control' */
  uint8_T st;                          /* '<S33>/position control' */
  uint8_T vxy_reset_counter_previous;  /* '<S31>/velocity_pid' */
  uint8_T vz_reset_counter_previous;   /* '<S31>/velocity_pid' */
  uint8_T state;                       /* '<S30>/update_takeoff_status' */
  uint8_T xy_reset_counter__i;         /* '<S30>/DisturbanceFEstimator' */
  uint8_T z_reset_counter__i;          /* '<S30>/DisturbanceFEstimator' */
  uint8_T vxy_reset_counter__p;        /* '<S30>/DisturbanceFEstimator' */
  uint8_T vz_reset_counter__n;         /* '<S30>/DisturbanceFEstimator' */
  uint8_T quat_reset_counter_;         /* '<S15>/MATLAB Function' */
  boolean_T initialized_not_empty;     /* '<S115>/df' */
  boolean_T time_start_not_empty;      /* '<S115>/MissionSwtich' */
  boolean_T acc_last_not_empty;        /* '<S115>/MissionSwtich' */
  boolean_T takeoff_yaw_not_empty;     /* '<S115>/MissionSwtich' */
  boolean_T xy_reset_counter__not_empty;/* '<S115>/MissionSwtich' */
  boolean_T z_reset_counter__not_empty;/* '<S115>/MissionSwtich' */
  boolean_T vxy_reset_counter__not_empty;/* '<S115>/MissionSwtich' */
  boolean_T vz_reset_counter__not_empty;/* '<S115>/MissionSwtich' */
  boolean_T heading_reset_counter__not_empt;/* '<S115>/MissionSwtich' */
  boolean_T valid_cnt_not_empty;       /* '<S115>/MissionSwtich' */
  boolean_T vision_active;             /* '<S115>/MissionSwtich' */
  boolean_T fallback_active;           /* '<S115>/MissionSwtich' */
  boolean_T last_landing_enable;       /* '<S115>/MissionSwtich' */
  boolean_T yaw_ref_initialized;       /* '<S115>/MissionSwtich' */
  boolean_T lpf_wz_sp_not_empty;       /* '<S114>/Lowpass3' */
  boolean_T lpf_vel_sp_not_empty;      /* '<S114>/Lowpass2' */
  boolean_T lpf_vel_sp_not_empty_e;    /* '<S114>/Lowpass1' */
  boolean_T lpf_vel_sp_not_empty_k;    /* '<S114>/Lowpass' */
  boolean_T pos_x1_not_empty;          /* '<S33>/position control' */
  boolean_T pos_y1_not_empty;          /* '<S33>/position control' */
  boolean_T pos_z1_not_empty;          /* '<S33>/position control' */
  boolean_T yaw_d__not_empty;          /* '<S33>/position control' */
  boolean_T hold_z_flag;               /* '<S33>/position control' */
  boolean_T is_braking_z;              /* '<S33>/position control' */
  boolean_T hold_z_timestamp_not_empty;/* '<S33>/position control' */
  boolean_T hold_vz_not_empty;         /* '<S33>/position control' */
  boolean_T hold_pz_not_empty;         /* '<S33>/position control' */
  boolean_T xy_reset_counter__not_empty_f;/* '<S33>/position control' */
  boolean_T traj_gen_xy_not_empty;     /* '<S33>/position control' */
  boolean_T lw_pos_mode__not_empty;    /* '<S33>/position control' */
  boolean_T lpf_stick_x_not_empty;     /* '<S33>/position control' */
  boolean_T lpf_stick_y_not_empty;     /* '<S33>/position control' */
  boolean_T filter_state_not_empty;    /* '<S33>/position control' */
  boolean_T st_not_empty;              /* '<S33>/position control' */
  boolean_T last_landing_enable_e;     /* '<S33>/position control' */
  boolean_T motion_evidence_armed_prev;/* '<S33>/position control' */
  boolean_T stage_blocked;             /* '<S33>/position control' */
  boolean_T hist_init;                 /* '<S33>/position control' */
  boolean_T pid_x_not_empty;           /* '<S31>/velocity_pid' */
  boolean_T initialized_not_empty_a;   /* '<S30>/update_takeoff_status' */
  boolean_T hys_state;                 /* '<S30>/update_takeoff_status' */
  boolean_T hys_requested_state;       /* '<S30>/update_takeoff_status' */
  boolean_T lpf_d_est_x_not_empty;     /* '<S30>/MATLAB Function3' */
  boolean_T lpf_d_est_y_not_empty;     /* '<S30>/MATLAB Function3' */
  boolean_T lpf_d_est_z_not_empty;     /* '<S30>/MATLAB Function3' */
  boolean_T xw_hist_not_empty;         /* '<S30>/MATLAB Function1' */
  boolean_T was_flatness;              /* '<S30>/MATLAB Function1' */
  boolean_T state_freefall;            /* '<S30>/LandDetector' */
  boolean_T state_contact;             /* '<S30>/LandDetector' */
  boolean_T state_maybe;               /* '<S30>/LandDetector' */
  boolean_T state_landed;              /* '<S30>/LandDetector' */
  boolean_T is_landed_prev;            /* '<S30>/LandDetector' */
  boolean_T is_maybe_prev;             /* '<S30>/LandDetector' */
  boolean_T pk_not_empty;              /* '<S30>/DisturbanceFEstimator' */
  boolean_T vk_not_empty;              /* '<S30>/DisturbanceFEstimator' */
  boolean_T xy_reset_counter__not_empty_a;/* '<S30>/DisturbanceFEstimator' */
  boolean_T z_reset_counter__not_empty_c;/* '<S30>/DisturbanceFEstimator' */
  boolean_T vxy_reset_counter__not_empty_j;/* '<S30>/DisturbanceFEstimator' */
  boolean_T vz_reset_counter__not_empty_l;/* '<S30>/DisturbanceFEstimator' */
  boolean_T pos_valid_prev[3];         /* '<S30>/DisturbanceFEstimator' */
  boolean_T vel_valid_prev[3];         /* '<S30>/DisturbanceFEstimator' */
  boolean_T initialized;               /* '<S30>/DisturbanceFEstimator' */
  boolean_T initialized_l;             /* '<S30>/AirspeedEstimator' */
  boolean_T fast_enough_latched;       /* '<S30>/AirspeedEstimator' */
  boolean_T lpf_alpha_des_x_not_empty; /* '<S21>/rates_control_to_torque' */
  boolean_T lpf_alpha_des_y_not_empty; /* '<S21>/rates_control_to_torque' */
  boolean_T lpf_alpha_des_z_not_empty; /* '<S21>/rates_control_to_torque' */
  boolean_T alpha_des_cutoff_xy_last_not_em;/* '<S21>/rates_control_to_torque' */
  boolean_T alpha_des_cutoff_z_last_not_emp;/* '<S21>/rates_control_to_torque' */
  boolean_T sample_time_alpha_lpf_last_not_;/* '<S21>/rates_control_to_torque' */
  boolean_T pid_x_not_empty_h;         /* '<S21>/rates_control_to_torque' */
  boolean_T was_enable;                /* '<S21>/MATLAB Function' */
  boolean_T was_enable_not_empty;      /* '<S21>/MATLAB Function' */
  boolean_T lpf_d_est_x_not_empty_m;   /* '<S21>/Lowpass' */
  boolean_T lpf_d_est_y_not_empty_p;   /* '<S21>/Lowpass' */
  boolean_T lpf_d_est_z_not_empty_a;   /* '<S21>/Lowpass' */
  boolean_T tau_omegak_not_empty;      /* '<S21>/DisturbanceEstimator' */
  boolean_T lpf_omegam_not_empty;      /* '<S21>/DisturbanceEstimator' */
  boolean_T T_filtered_state_not_empty;/* '<S21>/DisturbanceEstimator' */
  boolean_T altitude_yaw_d__not_empty; /* '<S15>/MATLAB Function' */
  boolean_T quat_reset_counter__not_empty;/* '<S15>/MATLAB Function' */
  boolean_T delta_l_not_empty;         /* '<S92>/IntegratedMixer' */
  boolean_T timestamp_last_us_not_empty;/* '<S10>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_c_T sf_hrt_timestamp_knt;/* '<S151>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_c_T sf_hrt_timestamp_p;/* '<S156>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_1_T sf_hrt_timestamp_kn;/* '<S112>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_1_T sf_hrt_timestamp_k;/* '<S32>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_1_T sf_hrt_timestamp_a;/* '<S17>/hrt_timestamp' */
  DW_hrt_timestamp_LW_ctrl_V1_1_T sf_hrt_timestamp_d;/* '<S94>/hrt_timestamp' */
} DW_LW_ctrl_V1_11_260814_T;

/* Invariant block signals (default storage) */
typedef struct {
  const real32_T Gain[3];              /* '<S90>/Gain' */
} ConstB_LW_ctrl_V1_11_260814_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Pooled Parameter (Expression: airframe_params)
   * Referenced by:
   *   '<S92>/IntegratedMixer'
   *   '<S115>/df'
   *   '<S30>/AirspeedEstimator'
   *   '<S30>/DisturbanceFEstimator'
   *   '<S30>/LandDetector'
   *   '<S30>/MATLAB Function1'
   *   '<S100>/MATLAB Function'
   *   '<S104>/pwm_out2'
   *   '<S160>/getIncid'
   *   '<S20>/roll_yaw_control'
   *   '<S21>/DisturbanceEstimator'
   *   '<S21>/rates_control_to_torque'
   */
  BusAirframeParam pooled1;

  /* Computed Parameter: Saturation_LowerSat
   * Referenced by: '<S93>/Saturation'
   */
  real32_T Saturation_LowerSat[16];

  /* Expression: MAX_M
   * Referenced by: '<S21>/rates_control_to_torque'
   */
  real32_T rates_control_to_torque_MAX_M[3];

  /* Expression: MIN_M
   * Referenced by: '<S21>/rates_control_to_torque'
   */
  real32_T rates_control_to_torque_MIN_M[3];
} ConstP_LW_ctrl_V1_11_260814_T;

/* Code_Instrumentation_Declarations_Placeholder */

/* Real-time Model Data Structure */
struct tag_RTM_LW_ctrl_V1_11_260814_T {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
  } Timing;
};

/* Block signals (default storage) */
extern B_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_B;

/* Block states (default storage) */
extern DW_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_DW;
extern const ConstB_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_ConstP;

/* Model entry point functions */
extern void LW_ctrl_V1_11_260814_initialize(void);
extern void LW_ctrl_V1_11_260814_step(void);
extern void LW_ctrl_V1_11_260814_terminate(void);

/* Exported data declaration */

/* Declaration of data with custom storage class MTD */
extern real32_T LNDMC_ABL_ALT;         /* Referenced by:
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S33>/position control'
                                        */
extern real32_T LNDMC_ABL_DEC;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_DSUP;    /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_DVZ;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_DZ;      /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_SUP;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_T;       /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_TMIN;    /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_VZ;      /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_VZSP;    /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_VZUP;    /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_WIN;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_ZSP;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ABL_ZUP;     /* Referenced by: '<S33>/position control' */
extern real32_T LNDMC_ALT_GND;         /* Referenced by: '<S30>/LandDetector' */
extern real32_T LNDMC_GND_F;           /* Referenced by: '<S30>/LandDetector' */
extern real32_T LNDMC_ROT_MAX;         /* Referenced by: '<S30>/LandDetector' */
extern real32_T LNDMC_TRIG_TIME;       /* Referenced by: '<S30>/LandDetector' */
extern real32_T LNDMC_XY_VEL_MAX;      /* Referenced by:
                                        * '<S30>/LandDetector'
                                        * '<S33>/position control'
                                        */
extern real32_T LNDMC_Z_VEL_MAX;       /* Referenced by: '<S30>/LandDetector' */
extern real32_T LW_ACCXYSP_FZ;         /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_ACCX_FF;        /* Referenced by: '<S30>/MATLAB Function1' */
extern real32_T LW_ACCY_FF;        /* Referenced by: '<S30>/MATLAB Function1' */
extern real32_T LW_ACCZSP_FZ;          /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_ACCZ_FF;        /* Referenced by: '<S30>/MATLAB Function1' */
extern real32_T LW_ACRO_P_MAX;         /* Referenced by: '<S114>/Gain7' */
extern real32_T LW_ACRO_R_MAX;         /* Referenced by: '<S114>/Gain6' */
extern real32_T LW_ACRO_Y_MAX;         /* Referenced by: '<S114>/Gain2' */
extern real32_T LW_ADOT_XY_FZ;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_ADOT_Z_FZ;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_AERO_C0;            /* Referenced by:
                                        * '<S115>/df'
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S30>/MATLAB Function1'
                                        */
extern real32_T LW_AERO_C1;            /* Referenced by:
                                        * '<S115>/df'
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S30>/MATLAB Function1'
                                        */
extern int32_T LW_AERO_COM;            /* Referenced by:
                                        * '<S7>/MATLAB Function4'
                                        * '<S92>/IntegratedMixer'
                                        * '<S30>/MATLAB Function1'
                                        */
extern real32_T LW_AERO_Y0;            /* Referenced by:
                                        * '<S115>/df'
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S30>/MATLAB Function1'
                                        */
extern real32_T LW_ALPHA_CTRL_EN;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_COOR_TURN;       /* Referenced by: '<S7>/MATLAB Function4' */
extern real32_T LW_CT_K;               /* Referenced by: '<S104>/toThrottle' */
extern real32_T LW_DSER;            /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_DTHR;            /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_FEST_CF;        /* Referenced by: '<S30>/MATLAB Function3' */
extern real32_T LW_FEST_EN;        /* Referenced by: '<S30>/MATLAB Function1' */
extern real32_T LW_FEST_LAMXY;/* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_FEST_LAMZ; /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_FEST_P;         /* Referenced by: '<S30>/MATLAB Function1' */
extern real32_T LW_FEST_SIGXY;/* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_FEST_SIGZ; /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_FEST_TDT;           /* Referenced by:
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        */
extern real32_T LW_FEST_WNXY; /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_FEST_WNZ;  /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern int32_T LW_F_MAP;               /* Referenced by:
                                        * '<S7>/MATLAB Function4'
                                        * '<S30>/MATLAB Function1'
                                        */
extern real32_T LW_JX_K;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_JY_K;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_JZ_K;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_K_ALPHA_XY;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_K_ALPHA_Z;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_LBCL_D;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_LBCM_D;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_LED;                /* Referenced by: '<S160>/LW_LED' */
extern real32_T LW_MAN_PITCH_DZ;    /* Referenced by: '<S114>/pitch_deadzone' */
extern real32_T LW_MAN_PITCH_MAX;    /* Referenced by: '<S114>/man_pitch_max' */
extern real32_T LW_MAN_ROLL_DZ;      /* Referenced by: '<S114>/roll_deadzone' */
extern real32_T LW_MAN_ROLL_MAX;      /* Referenced by: '<S114>/man_roll_max' */
extern real32_T LW_MAN_WX_DZ;    /* Referenced by: '<S114>/rollrate_deadzone' */
extern real32_T LW_MAN_WY_DZ;   /* Referenced by: '<S114>/pitchrate_deadzone' */
extern real32_T LW_MAN_WZ_DZ;     /* Referenced by: '<S114>/yawrate_deadzone' */
extern real32_T LW_MASS_K;             /* Referenced by:
                                        * '<S114>/Gain4'
                                        * '<S115>/df'
                                        * '<S30>/AirspeedEstimator'
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S30>/LandDetector'
                                        * '<S30>/MATLAB Function1'
                                        */
extern real32_T LW_MIXER_GAM;       /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_OB_CIRCLE_R;      /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T LW_OB_CIRCLE_V;      /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T LW_OB_CIRCLE_W;      /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T LW_OB_TYPE;            /* Referenced by:
                                        * '<S115>/MissionSwtich'
                                        * '<S21>/Constant'
                                        */
extern real32_T LW_PITCH_P;        /* Referenced by: '<S20>/roll_yaw_control' */
extern int32_T LW_POS_MODE;        /* Referenced by: '<S33>/position control' */
extern real32_T LW_REX_YAW;        /* Referenced by: '<S33>/position control' */
extern real32_T LW_ROLL_P;         /* Referenced by: '<S20>/roll_yaw_control' */
extern real32_T LW_SERVOL_REV;         /* Referenced by: '<S104>/pwm_out2' */
extern real32_T LW_SERVOR_CON;      /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_SERVOR_REV;         /* Referenced by: '<S104>/pwm_out2' */
extern real32_T LW_SER_LP;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_TAUXYSP_TM;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_TAUZSP_TM;
                            /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_TEST_CFXY;          /* Referenced by: '<S21>/Lowpass' */
extern real32_T LW_TEST_CFZ;           /* Referenced by: '<S21>/Lowpass' */
extern real32_T LW_TEST_EN;            /* Referenced by:
                                        * '<S92>/IntegratedMixer'
                                        * '<S21>/rates_control_to_torque'
                                        */
extern real32_T LW_TEST_LAMXY; /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_LAMZ;  /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_SIGXY; /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_SIGZ;  /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_TDT;           /* Referenced by:
                                        * '<S21>/DisturbanceEstimator'
                                        * '<S21>/rates_control_to_torque'
                                        */
extern real32_T LW_TEST_WBCF;  /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_WNXY;  /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_TEST_WNZ;   /* Referenced by: '<S21>/DisturbanceEstimator' */
extern real32_T LW_THR_HOVER;          /* Referenced by: '<S114>/Constant2' */
extern real32_T LW_THR_LOW;            /* Referenced by:
                                        * '<S92>/IntegratedMixer'
                                        * '<S15>/MATLAB Function'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T LW_THR_MIN;            /* Referenced by:
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S30>/LandDetector'
                                        */
extern real32_T LW_TILTMAX_AIR;        /* Referenced by:
                                        * '<S15>/MATLAB Function'
                                        * '<S30>/MATLAB Function1'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T LW_USE_SER;         /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_VA_TMAX;            /* Referenced by:
                                        * '<S92>/IntegratedMixer'
                                        * '<S20>/roll_yaw_control'
                                        * '<S21>/rates_control_to_torque'
                                        */
extern real32_T LW_VA_TMIN;            /* Referenced by:
                                        * '<S92>/IntegratedMixer'
                                        * '<S15>/MATLAB Function'
                                        * '<S30>/MATLAB Function1'
                                        * '<S33>/position control'
                                        * '<S20>/roll_yaw_control'
                                        * '<S21>/rates_control_to_torque'
                                        */
extern real32_T LW_WX_D;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WX_FF;     /* Referenced by: '<S21>/attrates_ref_selector' */
extern real32_T LW_WX_I;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WX_MAX;             /* Referenced by:
                                        * '<S33>/position control'
                                        * '<S20>/roll_yaw_control'
                                        */
extern real32_T LW_WX_N;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WX_P;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WX_P2;   /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WY_D;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WY_DE_MAX;      /* Referenced by: '<S20>/roll_yaw_control' */
extern real32_T LW_WY_FF;     /* Referenced by: '<S21>/attrates_ref_selector' */
extern real32_T LW_WY_I;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WY_MAX;         /* Referenced by: '<S20>/roll_yaw_control' */
extern real32_T LW_WY_N;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WY_P;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WY_P2;   /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WZSP_CF;            /* Referenced by: '<S114>/Lowpass3' */
extern real32_T LW_WZ_D;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WZ_FF;              /* Referenced by:
                                        * '<S20>/roll_yaw_control'
                                        * '<S21>/attrates_ref_selector'
                                        */
extern real32_T LW_WZ_I;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WZ_MAX;             /* Referenced by:
                                        * '<S33>/position control'
                                        * '<S20>/roll_yaw_control'
                                        */
extern real32_T LW_WZ_N;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_WZ_P;    /* Referenced by: '<S21>/rates_control_to_torque' */
extern real32_T LW_W_FZ;            /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_W_SER;           /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_W_SERH;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_W_TAOX;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_W_TAOY;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_W_TAOZ;          /* Referenced by: '<S92>/IntegratedMixer' */
extern real32_T LW_XY_VELSP_CF;        /* Referenced by:
                                        * '<S114>/Lowpass'
                                        * '<S114>/Lowpass1'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_X_ACCEL_MAX;        /* Referenced by:
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T LW_X_ACCEL_MIN;
                              /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_X_FF;           /* Referenced by: '<S33>/position control' */
extern real32_T LW_X_JERK_MAX;         /* Referenced by:
                                        * '<S31>/velocity_pid'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_X_P;            /* Referenced by: '<S33>/position control' */
extern real32_T LW_X_VELD_FZ;          /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_X_VEL_D;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_X_VEL_DZ;           /* Referenced by: '<S114>/vx_deadzone' */
extern real32_T LW_X_VEL_I;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_X_VEL_MAX;          /* Referenced by:
                                        * '<S114>/max_vx'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_X_VEL_P;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_YAWMODE2_VA;        /* Referenced by:
                                        * '<S115>/MissionSwtich'
                                        * '<S115>/df'
                                        * '<S30>/MATLAB Function1'
                                        * '<S33>/position control'
                                        * '<S20>/roll_yaw_control'
                                        */
extern int32_T LW_YAW_MODE;            /* Referenced by:
                                        * '<S7>/MATLAB Function4'
                                        * '<S115>/MissionSwtich'
                                        * '<S30>/MATLAB Function1'
                                        * '<S33>/position control'
                                        * '<S20>/roll_yaw_control'
                                        */
extern real32_T LW_YAW_P;          /* Referenced by: '<S20>/roll_yaw_control' */
extern real32_T LW_Y_ACCEL_MAX;        /* Referenced by:
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S31>/velocity_pid'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_Y_ACCEL_MIN;
                              /* Referenced by: '<S30>/DisturbanceFEstimator' */
extern real32_T LW_Y_FF;           /* Referenced by: '<S33>/position control' */
extern real32_T LW_Y_JERK_MAX;         /* Referenced by:
                                        * '<S31>/velocity_pid'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_Y_P;            /* Referenced by: '<S33>/position control' */
extern real32_T LW_Y_VELD_FZ;          /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Y_VEL_D;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Y_VEL_DZ;           /* Referenced by: '<S114>/vy_deadzone' */
extern real32_T LW_Y_VEL_I;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Y_VEL_MAX;          /* Referenced by: '<S114>/max_vy' */
extern real32_T LW_Y_VEL_P;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Z_ACCEL_LOW;        /* Referenced by:
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T LW_Z_ACCEL_UP;         /* Referenced by:
                                        * '<S30>/DisturbanceFEstimator'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T LW_Z_FF;           /* Referenced by: '<S33>/position control' */
extern real32_T LW_Z_JERK_MAX;         /* Referenced by:
                                        * '<S31>/velocity_pid'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_Z_P;            /* Referenced by: '<S33>/position control' */
extern real32_T LW_Z_VELD_FZ;          /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Z_VELSP_CF;         /* Referenced by: '<S114>/Lowpass2' */
extern real32_T LW_Z_VEL_D;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Z_VEL_DZ;           /* Referenced by: '<S114>/vz_deadzone' */
extern real32_T LW_Z_VEL_I;            /* Referenced by: '<S31>/velocity_pid' */
extern real32_T LW_Z_VEL_MAX_UP;       /* Referenced by:
                                        * '<S114>/max_vz'
                                        * '<S33>/position control'
                                        */
extern real32_T LW_Z_VEL_P;            /* Referenced by:
                                        * '<S30>/update_takeoff_status'
                                        * '<S31>/velocity_pid'
                                        */
extern real32_T MPC_TKO_RAMP_T;
                              /* Referenced by: '<S30>/update_takeoff_status' */
extern real32_T MPC_TKO_RAMP_T0;
                              /* Referenced by: '<S30>/update_takeoff_status' */
extern real32_T MPC_TKO_SPEED;/* Referenced by: '<S30>/update_takeoff_status' */
extern real32_T VL_MAX_ABS_JUMP;     /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T VL_MAX_POS_ERR;      /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T VL_MAX_REF_SPD;      /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T VL_MIN_Z;            /* Referenced by: '<S115>/MissionSwtich' */
extern real32_T VL_WZ_MAX;           /* Referenced by: '<S115>/MissionSwtich' */
extern int32_T WE_EN;                  /* Referenced by:
                                        * '<S160>/Constant1'
                                        * '<S160>/Constant2'
                                        */

/* Real-time Model object */
extern RT_MODEL_LW_ctrl_V1_11_260814_T *const LW_ctrl_V1_11_260814_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S20>/Gain' : Unused code path elimination
 * Block '<S20>/Gain1' : Unused code path elimination
 * Block '<S71>/Constant1' : Unused code path elimination
 * Block '<S76>/Assertion' : Unused code path elimination
 * Block '<S78>/Assertion' : Unused code path elimination
 * Block '<S73>/Constant1' : Unused code path elimination
 * Block '<S79>/Assertion' : Unused code path elimination
 * Block '<S81>/Assertion' : Unused code path elimination
 * Block '<S160>/uORB Read Function-Call Trigger3' : Unused code path elimination
 * Block '<S160>/uORB Read Function-Call Trigger4' : Unused code path elimination
 * Block '<S10>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S29>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S29>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S29>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S29>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S29>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S38>/Reshape 3x3 -> 9' : Reshape block reduction
 * Block '<S74>/Reshape' : Reshape block reduction
 * Block '<S83>/Reshape' : Reshape block reduction
 * Block '<S90>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S90>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S90>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S90>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S90>/Data Type Conversion5' : Eliminate redundant data type conversion
 * Block '<S104>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S117>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S117>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S117>/Data Type Conversion6' : Eliminate redundant data type conversion
 * Block '<S117>/Data Type Conversion7' : Eliminate redundant data type conversion
 * Block '<S114>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S114>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S114>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion4' : Eliminate redundant data type conversion
 * Block '<S151>/Data Type Conversion8' : Eliminate redundant data type conversion
 * Block '<S156>/Data Type Conversion' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'LW_ctrl_V1_11_260814'
 * '<S1>'   : 'LW_ctrl_V1_11_260814/Output'
 * '<S2>'   : 'LW_ctrl_V1_11_260814/Scheduler'
 * '<S3>'   : 'LW_ctrl_V1_11_260814/control'
 * '<S4>'   : 'LW_ctrl_V1_11_260814/mixer'
 * '<S5>'   : 'LW_ctrl_V1_11_260814/navigator'
 * '<S6>'   : 'LW_ctrl_V1_11_260814/rc_receive'
 * '<S7>'   : 'LW_ctrl_V1_11_260814/state_machine'
 * '<S8>'   : 'LW_ctrl_V1_11_260814/states_update'
 * '<S9>'   : 'LW_ctrl_V1_11_260814/Output/OutputSelect'
 * '<S10>'  : 'LW_ctrl_V1_11_260814/Output/OutputSelect/FLY'
 * '<S11>'  : 'LW_ctrl_V1_11_260814/Output/OutputSelect/FLY/Compare To Constant'
 * '<S12>'  : 'LW_ctrl_V1_11_260814/Output/OutputSelect/FLY/hrt_timestamp'
 * '<S13>'  : 'LW_ctrl_V1_11_260814/control/attitude_control'
 * '<S14>'  : 'LW_ctrl_V1_11_260814/control/position_control'
 * '<S15>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/att_ref_selector'
 * '<S16>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control'
 * '<S17>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/get_timestamp'
 * '<S18>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/logger'
 * '<S19>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/att_ref_selector/MATLAB Function'
 * '<S20>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/attitude_control'
 * '<S21>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control'
 * '<S22>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/attitude_control/roll_yaw_control'
 * '<S23>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control/DisturbanceEstimator'
 * '<S24>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control/Lowpass'
 * '<S25>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control/MATLAB Function'
 * '<S26>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control/attrates_ref_selector'
 * '<S27>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/do_attitude_control/rates_control/rates_control_to_torque'
 * '<S28>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/get_timestamp/hrt_timestamp'
 * '<S29>'  : 'LW_ctrl_V1_11_260814/control/attitude_control/logger/HIL&FLY'
 * '<S30>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear'
 * '<S31>'  : 'LW_ctrl_V1_11_260814/control/position_control/do_position_control'
 * '<S32>'  : 'LW_ctrl_V1_11_260814/control/position_control/get_timestamp'
 * '<S33>'  : 'LW_ctrl_V1_11_260814/control/position_control/local_pos_ref_selector'
 * '<S34>'  : 'LW_ctrl_V1_11_260814/control/position_control/logger'
 * '<S35>'  : 'LW_ctrl_V1_11_260814/control/position_control/nan_inf0'
 * '<S36>'  : 'LW_ctrl_V1_11_260814/control/position_control/nan_inf_q'
 * '<S37>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/AirspeedEstimator'
 * '<S38>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions'
 * '<S39>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/DisturbanceFEstimator'
 * '<S40>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/LandDetector'
 * '<S41>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/MATLAB Function1'
 * '<S42>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/MATLAB Function3'
 * '<S43>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/update_takeoff_status'
 * '<S44>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace'
 * '<S45>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Positive Trace'
 * '<S46>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM'
 * '<S47>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/trace(DCM)'
 * '<S48>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)'
 * '<S49>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)'
 * '<S50>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)'
 * '<S51>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/diag(DCM)'
 * '<S52>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) -sin(theta)'
 * '<S53>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(theta)sin(phi) - (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S54>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(theta)sin(psi) + (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S55>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/if s~=0; s=0.5//s'
 * '<S56>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/u(1) -(u(5)+u(9)) +1'
 * '<S57>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) +sin(theta)'
 * '<S58>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(theta)sin(phi) + (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S59>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(theta)sin(psi) + (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S60>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/if s~=0; s=0.5//s'
 * '<S61>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/u(5) -(u(1)+u(9)) +1'
 * '<S62>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) -sin(theta)'
 * '<S63>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(theta)sin(phi) + (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S64>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(theta)sin(psi) - (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S65>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/if s~=0; s=0.5//s'
 * '<S66>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/u(9) -(u(1)+u(5)) +1'
 * '<S67>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) +sin(theta)'
 * '<S68>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(theta)sin(phi) - (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S69>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(theta)sin(psi) - (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S70>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error'
 * '<S71>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/Else If Not Orthogonal'
 * '<S72>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/Else No Action'
 * '<S73>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/If Not Proper'
 * '<S74>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/isNotOrthogonal'
 * '<S75>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/isNotProper'
 * '<S76>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/Else If Not Orthogonal/Error'
 * '<S77>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/Else If Not Orthogonal/None'
 * '<S78>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/Else If Not Orthogonal/Warning'
 * '<S79>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/If Not Proper/Error'
 * '<S80>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/If Not Proper/None'
 * '<S81>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/If Not Proper/Warning'
 * '<S82>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/isNotOrthogonal/transpose*dcm ~= eye(3)'
 * '<S83>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/isNotProper/Determinant of 3x3 Matrix'
 * '<S84>'  : 'LW_ctrl_V1_11_260814/control/position_control/Nonlinear/Direction Cosine Matrix  to Quaternions/Validate DCM/If Warning//Error/isNotProper/determinant does not equal 1'
 * '<S85>'  : 'LW_ctrl_V1_11_260814/control/position_control/do_position_control/velocity_pid'
 * '<S86>'  : 'LW_ctrl_V1_11_260814/control/position_control/get_timestamp/hrt_timestamp'
 * '<S87>'  : 'LW_ctrl_V1_11_260814/control/position_control/local_pos_ref_selector/nan_inf_mode'
 * '<S88>'  : 'LW_ctrl_V1_11_260814/control/position_control/local_pos_ref_selector/nan_inf_mode1'
 * '<S89>'  : 'LW_ctrl_V1_11_260814/control/position_control/local_pos_ref_selector/position control'
 * '<S90>'  : 'LW_ctrl_V1_11_260814/control/position_control/logger/HIL&FLY'
 * '<S91>'  : 'LW_ctrl_V1_11_260814/control/position_control/logger/HIL&FLY/MATLAB Function'
 * '<S92>'  : 'LW_ctrl_V1_11_260814/mixer/Subsystem Reference'
 * '<S93>'  : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle'
 * '<S94>'  : 'LW_ctrl_V1_11_260814/mixer/get_timestamp'
 * '<S95>'  : 'LW_ctrl_V1_11_260814/mixer/logger'
 * '<S96>'  : 'LW_ctrl_V1_11_260814/mixer/nan_inf0'
 * '<S97>'  : 'LW_ctrl_V1_11_260814/mixer/nan_inf1'
 * '<S98>'  : 'LW_ctrl_V1_11_260814/mixer/Subsystem Reference/IntegratedMixer'
 * '<S99>'  : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/Compare To Constant'
 * '<S100>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/actuator_cmd_selector'
 * '<S101>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/nan_inf0'
 * '<S102>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/nan_inf1'
 * '<S103>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/nan_inf_mode'
 * '<S104>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/toThrottle'
 * '<S105>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/actuator_cmd_selector/MATLAB Function'
 * '<S106>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/actuator_cmd_selector/nan_inf_mode'
 * '<S107>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/actuator_cmd_selector/nan_inf_mode1'
 * '<S108>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/toThrottle/pwm_out2'
 * '<S109>' : 'LW_ctrl_V1_11_260814/mixer/forceToThrottle/toThrottle/toThrottle'
 * '<S110>' : 'LW_ctrl_V1_11_260814/mixer/get_timestamp/hrt_timestamp'
 * '<S111>' : 'LW_ctrl_V1_11_260814/mixer/logger/HIL&FLY'
 * '<S112>' : 'LW_ctrl_V1_11_260814/navigator/get_timestamp'
 * '<S113>' : 'LW_ctrl_V1_11_260814/navigator/logger'
 * '<S114>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual'
 * '<S115>' : 'LW_ctrl_V1_11_260814/navigator/navigator_offboard'
 * '<S116>' : 'LW_ctrl_V1_11_260814/navigator/get_timestamp/hrt_timestamp'
 * '<S117>' : 'LW_ctrl_V1_11_260814/navigator/logger/HIL&FLY'
 * '<S118>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/Lowpass'
 * '<S119>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/Lowpass1'
 * '<S120>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/Lowpass2'
 * '<S121>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/Lowpass3'
 * '<S122>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/f1'
 * '<S123>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf0'
 * '<S124>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf1'
 * '<S125>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf10'
 * '<S126>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf11'
 * '<S127>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf2'
 * '<S128>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf3'
 * '<S129>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf4'
 * '<S130>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf5'
 * '<S131>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf6'
 * '<S132>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf7'
 * '<S133>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf8'
 * '<S134>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/nan_inf9'
 * '<S135>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/pitch_deadzone'
 * '<S136>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/pitchrate_deadzone'
 * '<S137>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/pitchrate_deadzone1'
 * '<S138>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/roll_deadzone'
 * '<S139>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/rollrate_deadzone'
 * '<S140>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/rollrate_deadzone1'
 * '<S141>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/vx_deadzone'
 * '<S142>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/vy_deadzone'
 * '<S143>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/vz_deadzone'
 * '<S144>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/yawrate_deadzone'
 * '<S145>' : 'LW_ctrl_V1_11_260814/navigator/navigator_manual/yawrate_deadzone1'
 * '<S146>' : 'LW_ctrl_V1_11_260814/navigator/navigator_offboard/MissionSwtich'
 * '<S147>' : 'LW_ctrl_V1_11_260814/navigator/navigator_offboard/df'
 * '<S148>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in'
 * '<S149>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in/HIL&FLY V1.14.3'
 * '<S150>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in/HIL&FLY V1.14.3/MATLAB Function'
 * '<S151>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in/HIL&FLY V1.14.3/logger'
 * '<S152>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in/HIL&FLY V1.14.3/logger/get_delta_t'
 * '<S153>' : 'LW_ctrl_V1_11_260814/rc_receive/manual_ctrl_in/HIL&FLY V1.14.3/logger/hrt_timestamp'
 * '<S154>' : 'LW_ctrl_V1_11_260814/state_machine/MATLAB Function4'
 * '<S155>' : 'LW_ctrl_V1_11_260814/state_machine/logger'
 * '<S156>' : 'LW_ctrl_V1_11_260814/state_machine/logger/HIL&FLY'
 * '<S157>' : 'LW_ctrl_V1_11_260814/state_machine/logger/HIL&FLY/get_delta_t'
 * '<S158>' : 'LW_ctrl_V1_11_260814/state_machine/logger/HIL&FLY/hrt_timestamp'
 * '<S159>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position'
 * '<S160>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3'
 * '<S161>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/LW_LED'
 * '<S162>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/getIncid'
 * '<S163>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/nan_inf'
 * '<S164>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/nan_inf0'
 * '<S165>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/nan_inf1'
 * '<S166>' : 'LW_ctrl_V1_11_260814/states_update/vehicle_local_position/HIL&FLY V1.14.3/nan_inf_q'
 */
#endif                                 /* RTW_HEADER_LW_ctrl_V1_11_260814_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
