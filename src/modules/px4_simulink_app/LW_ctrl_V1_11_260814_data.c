/*
 * File: LW_ctrl_V1_11_260814_data.c
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

#include "LW_ctrl_V1_11_260814.h"

/* Invariant block signals (default storage) */
const ConstB_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_ConstB = {
  { 0.0F, 0.0F, 0.0F }                 /* '<S90>/Gain' */
};

/* Constant parameters (default storage) */
const ConstP_LW_ctrl_V1_11_260814_T LW_ctrl_V1_11_260814_ConstP = {
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
  {
    44.5,

    { 6.049394, 0.0, 0.0, 0.0, 7.533715, 0.0, 0.0, 0.0, 12.239219 },
    1.6,
    0.6,
    1.06,
    0.55850536063818546,

    { -0.3490658503988659, -0.3490658503988659 },

    { 0.3490658503988659, 0.3490658503988659 },
    8.7266,

    { -0.5, 0.5, 0.0, 0.5, 0.5, 0.0 },
    -0.3490658503988659,
    -0.3490658503988659,
    0.0,
    0.3490658503988659,
    0.3490658503988659,
    0.0,
    2.0,

    { 0.83, 0.5, 0.0, -0.83, -0.5, 0.0, 0.83, -0.5, 0.0, -0.83, 0.5, 0.0 },

    { 0.0, 0.0, -1.0, 0.0, -0.0, -1.0, 0.0, -0.0, -1.0, 0.0, 0.0, -1.0 },

    { 1.0, 1.0, -1.0, -1.0 },
    4.0
  },

  /* Computed Parameter: Saturation_LowerSat
   * Referenced by: '<S93>/Saturation'
   */
  { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, -1.0F, -1.0F, -1.0F, -1.0F,
    -1.0F, -1.0F, -1.0F, -1.0F },

  /* Expression: MAX_M
   * Referenced by: '<S21>/rates_control_to_torque'
   */
  { 556.944031F, 924.527161F, 28.961092F },

  /* Expression: MIN_M
   * Referenced by: '<S21>/rates_control_to_torque'
   */
  { -556.944031F, -924.527161F, -28.9610901F }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
