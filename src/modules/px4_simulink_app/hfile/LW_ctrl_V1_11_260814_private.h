/*
 * File: LW_ctrl_V1_11_260814_private.h
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

#ifndef RTW_HEADER_LW_ctrl_V1_11_260814_private_h_
#define RTW_HEADER_LW_ctrl_V1_11_260814_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "LW_ctrl_V1_11_260814_types.h"
#include "LW_ctrl_V1_11_260814.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"

extern real32_T rt_atan2f_snf(real32_T u0, real32_T u1);
extern real32_T rt_roundf_snf(real32_T u);
extern real_T rt_roundd_snf(real_T u);
extern real32_T rt_hypotf_snf(real32_T u0, real32_T u1);
extern real_T rt_powd_snf(real_T u0, real_T u1);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);

/* Exported functions */
extern real32_T LW_ctrl_V_LowPassFilter1p_apply(LowPassFilter1p_LW_ctrl_V1_11_T *
  obj, real32_T sample);
extern real32_T LW_ctrl_LowPassFilter1p_apply_e(LowPassFilter1p_LW_ctrl_V1_11_T *
  obj, real32_T sample);
extern boolean_T LW_ctrl_V1_11_260814_notInfNan(const real32_T data[4]);
extern real32_T LW_ctrl_V1_11_26081_my_quat2yaw(const real32_T q[4]);
extern boolean_T LW_ctrl_V1_11_26081_notInfNan_e(const real32_T data[3]);
extern boolean_T LW_ctrl_V1_11_2608_notInfNan_e3(const real32_T data[2]);
extern real32_T LW_ctrl_V1_11_260814_wrap_pi(real32_T x);
extern void liftwingFlatnessSolver_localOrt(const real32_T Rin[9], real32_T
  Rfallback[9]);
extern void Double2MultiWord(real_T u1, uint32_T y[], int32_T n);
extern boolean_T uMultiWordEq(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
extern int32_T uMultiWordCmp(const uint32_T u1[], const uint32_T u2[], int32_T n);
extern void MultiWordSub(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
extern void uMultiWordShr(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
extern boolean_T uMultiWordGt(const uint32_T u1[], const uint32_T u2[], int32_T
  n);
extern void MultiWordAnd(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
  int32_T n);
extern void uMultiWordShl(const uint32_T u1[], int32_T n1, uint32_T n2, uint32_T
  y[], int32_T n);
extern void sMultiWord2uMultiWordSat(const uint32_T u1[], int32_T n1, uint32_T
  y[], int32_T n);
extern void uMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n);
extern real32_T uMultiWordSingle(const uint32_T u1[], int32_T n1, int32_T e1);
extern int32_T div_nde_s32_floor(int32_T numerator, int32_T denominator);
extern void LW_ctrl_V1_11_2608_nan_inf_mode(ControlMode rtu_u,
  B_nan_inf_mode_LW_ctrl_V1_11__T *localB);
extern void LW_ctrl_V1_11_260_nan_inf_mode1(FlightMode rtu_u,
  B_nan_inf_mode1_LW_ctrl_V1_11_T *localB);
extern void LW_ctrl_V1_11_260814_nan_inf0(const real32_T rtu_u[8],
  B_nan_inf0_LW_ctrl_V1_11_2608_T *localB);
extern void LW_ctrl_V1_11_260_hrt_timestamp(B_hrt_timestamp_LW_ctrl_V1_11_T
  *localB, DW_hrt_timestamp_LW_ctrl_V1_1_T *localDW);
extern void LW_ctrl_V1_11_260814_nan_inf0_m(real32_T rtu_u,
  B_nan_inf0_LW_ctrl_V1_11_26_g_T *localB);
extern void LW_ctrl_V1__pitchrate_deadzone1(real32_T rtu_u,
  B_pitchrate_deadzone1_LW_ctrl_T *localB);
extern void LW_ctrl_V1_11_2_hrt_timestamp_p(B_hrt_timestamp_LW_ctrl_V1__f_T
  *localB, DW_hrt_timestamp_LW_ctrl_V1_c_T *localDW);
extern void LW_ctrl_V1_11_260814_nan_inf(real32_T rtu_u, real32_T rtu_u_i,
  real32_T rtu_u_j, B_nan_inf_LW_ctrl_V1_11_26081_T *localB);

#endif                          /* RTW_HEADER_LW_ctrl_V1_11_260814_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
