/*
 * File: LW_ctrl_V1_14_261102_lw_s100_data.c
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

#include "LW_ctrl_V1_14_261102_lw_s100.h"

/* Invariant block signals (default storage) */
const ConstB_LW_ctrl_V1_14_261102_l_T LW_ctrl_V1_14_261102_lw__ConstB = {
  { 0.0F, 0.0F, 0.0F }                 /* '<S92>/Gain' */
};

/* Constant parameters (default storage) */
const ConstP_LW_ctrl_V1_14_261102_l_T LW_ctrl_V1_14_261102_lw__ConstP = {
  /* Pooled Parameter (Expression: airframe_params)
   * Referenced by:
   *   '<S94>/IntegratedMixer'
   *   '<S116>/df'
   *   '<S32>/AirspeedEstimator'
   *   '<S32>/DisturbanceFEstimator'
   *   '<S32>/LandDetector'
   *   '<S32>/MATLAB Function1'
   *   '<S102>/MATLAB Function'
   *   '<S106>/pwm_out2'
   *   '<S161>/getIncid'
   *   '<S22>/roll_yaw_control'
   *   '<S23>/DisturbanceEstimator'
   *   '<S23>/rates_control_to_torque'
   */
  {
    1.3,

    { 0.018546, 0.0, 0.0, 0.0, 0.017602, 0.0, 0.0, 0.0, 0.031924 },
    0.76,
    0.0872,
    0.113,
    0.0,

    { -0.3490658503988659, -0.3490658503988659 },

    { 0.3490658503988659, 0.3490658503988659 },
    10.0,

    { -0.5, 0.5, 0.0, 0.5, 0.5, 0.0 },
    -0.3490658503988659,
    -0.3490658503988659,
    0.0,
    0.3490658503988659,
    0.3490658503988659,
    0.0,
    2.0,
    0.5,

    { 0.17453292519943292, 0.17453292519943298, -0.17453292519943286,
      0.17453292519943295, 0.0, 0.0, 0.0, 0.0 },

    { 0.18, 0.19, 0.0, -0.18, -0.19, 0.0, 0.18, -0.19, 0.0, -0.18, 0.19, 0.0 },

    { 0.0, 0.0, -1.0, 0.0, -0.0, -1.0, 0.0, -0.0, -1.0, 0.0, 0.0, -1.0 },

    { 1.0, 1.0, -1.0, -1.0 },
    4.0
  },

  /* Expression: MAX_M
   * Referenced by: '<S23>/rates_control_to_torque'
   */
  { 2.96697879F, 2.81082225F, 0.195195958F },

  /* Expression: MIN_M
   * Referenced by: '<S23>/rates_control_to_torque'
   */
  { -2.96697879F, -2.81082225F, -0.195195973F }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
