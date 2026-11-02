/*
 * File: LW_ctrl_V1_14_261102_lw_s100_private.h
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

#ifndef RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_private_h_
#define RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "LW_ctrl_V1_14_261102_lw_s100_types.h"
#include "LW_ctrl_V1_14_261102_lw_s100.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

extern real32_T rt_atan2f_snf(real32_T u0, real32_T u1);
extern real32_T rt_roundf_snf(real32_T u);
extern real_T rt_roundd_snf(real_T u);
extern real32_T rt_hypotf_snf(real32_T u0, real32_T u1);
extern real_T rt_atan2d_snf(real_T u0, real_T u1);

/* Exported functions */
extern void LowPassFilter1p_set_cutoff_freq(LowPassFilter1p_LW_ctrl_V1_14_T *obj,
  real32_T sample_freq, real32_T cutoff_freq);
extern real32_T LW_ctrl_V_LowPassFilter1p_apply(LowPassFilter1p_LW_ctrl_V1_14_T *
  obj, real32_T sample);
extern real32_T LW_ctrl_LowPassFilter1p_apply_e(LowPassFilter1p_LW_ctrl_V1_14_T *
  obj, real32_T sample);
extern boolean_T LW_ctrl_V1_14_261102__notInfNan(const real32_T data[4]);
extern real32_T LW_ctrl_V1_14_26110_my_quat2yaw(const real32_T q[4]);
extern boolean_T LW_ctrl_V1_14_26110_notInfNan_e(const real32_T data[3]);
extern boolean_T LW_ctrl_V1_14_2611_notInfNan_e3(const real32_T data[2]);
extern real32_T LW_ctrl_V1_14_261102_lw_wrap_pi(real32_T x);
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
extern void LW_ctrl_V1_14_2611_nan_inf_mode(ControlMode rtu_u,
  B_nan_inf_mode_LW_ctrl_V1_14__T *localB);
extern void LW_ctrl_V1_14_261102_l_nan_inf0(const real32_T rtu_u[8],
  B_nan_inf0_LW_ctrl_V1_14_2611_T *localB);
extern void LW_ctrl_V1_14_261_hrt_timestamp(B_hrt_timestamp_LW_ctrl_V1_14_T
  *localB, DW_hrt_timestamp_LW_ctrl_V1_1_T *localDW);
extern void LW_ctrl_V1_14_261102_nan_inf0_m(real32_T rtu_u,
  B_nan_inf0_LW_ctrl_V1_14_26_g_T *localB);
extern void LW_ctrl_V1__pitchrate_deadzone1(real32_T rtu_u,
  B_pitchrate_deadzone1_LW_ctrl_T *localB);
extern void LW_ctrl_V1_14_2_hrt_timestamp_p(B_hrt_timestamp_LW_ctrl_V1__f_T
  *localB, DW_hrt_timestamp_LW_ctrl_V1_c_T *localDW);
extern void LW_ctrl_V1_14_261102_lw_nan_inf(real32_T rtu_u, real32_T rtu_u_i,
  real32_T rtu_u_j, B_nan_inf_LW_ctrl_V1_14_26110_T *localB);

#endif                  /* RTW_HEADER_LW_ctrl_V1_14_261102_lw_s100_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
