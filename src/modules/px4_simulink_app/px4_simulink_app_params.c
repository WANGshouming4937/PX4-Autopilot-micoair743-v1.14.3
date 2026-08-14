/****************************************************************************
 *
 *   Copyright (c) 2013-2016 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file px4_simulink_app_params.c
 * Hexacopter S550 controller parameters.
 *
 * @author wsh
 */
//******************************************************************************************************************//
//************************************************Vision Landing***********************************************//
//******************************************************************************************************************//

/**
 * Vision landing maximum allowed position error
 *
 * Maximum allowed distance between the current vehicle position and the
 * absolute position estimated from the vision landing target. If the distance
 * is larger than this value, the vision measurement is rejected.
 *
 * @unit m
 * @min 0.1
 * @max 20.0
 * @increment 0.1
 * @decimal 1
 * @group Vision Landing
 */
PARAM_DEFINE_FLOAT(VL_MAX_POS_ERR, 3.0f);

/**
 * Vision landing maximum allowed absolute position jump
 *
 * Maximum allowed jump between two consecutive valid absolute positions from
 * the vision landing target. If the jump is larger than this value, the vision
 * measurement is rejected.
 *
 * @unit m
 * @min 0.1
 * @max 10.0
 * @increment 0.1
 * @decimal 1
 * @group Vision Landing
 */
PARAM_DEFINE_FLOAT(VL_MAX_ABS_JUMP, 1.0f);

/**
 * Vision landing reference position speed limit
 *
 * Maximum speed used to smooth the vision landing position reference. A lower
 * value makes the transition smoother, but slows down convergence to the
 * target position.
 *
 * @unit m/s
 * @min 0.1
 * @max 5.0
 * @increment 0.1
 * @decimal 1
 * @group Vision Landing
 */
PARAM_DEFINE_FLOAT(VL_MAX_REF_SPD, 0.6f);

/**
 * Vision landing minimum test altitude
 *
 * Minimum altitude used during vision landing test. The vehicle will not
 * command a landing target lower than this height to avoid losing the marker
 * when it is too close.
 *
 * @unit m
 * @min 0.0
 * @max 100.0
 * @increment 0.1
 * @decimal 1
 * @group Vision Landing
 */
PARAM_DEFINE_FLOAT(VL_MIN_Z, 0.0f);

/**
 * Vision landing maximum yaw rate
 *
 * Maximum yaw rate allowed when the vision landing controller slews the
 * yaw setpoint toward the vision-provided yaw angle. This limit prevents
 * abrupt yaw setpoint changes during target acquisition, reacquisition, or
 * noisy target-yaw updates.
 *
 * @unit deg/s
 * @min 5.0
 * @max 150.0
 * @increment 1
 * @decimal 1
 * @group Vision Landing
 */
PARAM_DEFINE_FLOAT(VL_WZ_MAX, 40.0f);


//******************************************************************************************************************//
//************************************************Model Configuration***********************************************//
//******************************************************************************************************************//
/**
 * Parameter test
 *
 * Displayed in debug vector
 * 
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_LED, 0.0f);


/**
 * Aircraft mass scale factor
 *
 * Multiplies the nominal aircraft mass:
 * mass_used = LW_MASS_K * LW_MASS.
 * This parameter can be used to compensate for payload changes or
 * inaccuracies in the nominal aircraft mass.
 *
 * @min 0.5
 * @max 2.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_MASS_K, 1.0f);

/**
 * Thrust coefficient correction factor
 *
 * Scales the nominal thrust coefficient to compensate for differences
 * between the nominal propulsion model and the actual thrust performance.
 * A value of 1.0 uses the nominal thrust coefficient without correction.
 * Values below 1.0 indicate lower actual thrust than the nominal model,
 * while values above 1.0 indicate higher actual thrust.
 *
 * @min 0.5
 * @max 1.5
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_CT_K, 1.0f);


/**
 * Roll-axis moment of inertia scale factor
 *
 * Multiplies the nominal roll-axis moment of inertia:
 * Jx_used = LW_JX_K * Jx_nominal.
 *
 * @min 0.2
 * @max 3.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_JX_K, 1.0f);

/**
 * Pitch-axis moment of inertia scale factor
 *
 * Multiplies the nominal pitch-axis moment of inertia:
 * Jy_used = LW_JY_K * Jy_nominal.
 *
 * @min 0.2
 * @max 3.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_JY_K, 1.0f);

/**
 * Yaw-axis moment of inertia scale factor
 *
 * Multiplies the nominal yaw-axis moment of inertia:
 * Jz_used = LW_JZ_K * Jz_nominal.
 *
 * @unit
 * @min 0.2
 * @max 3.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_JZ_K, 1.0f);


/**
 * Installation angle between lifting wing and rotor plane
 *
 * @unit deg
 * @min 0.0
 * @max 90.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_ANGLE, 32.0f);

/**
 * Right aileron deflection direction calibration
 *
 * @min -1.0
 * @max 1.0
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOR_REV, 1.0f);

/**
 * Left aileron deflection direction calibration
 *
 * @min -1.0
 * @max 1.0
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOL_REV, 1.0f);

/**
 * Maximum deflection angle of right aileron
 *
 * @unit deg
 * @min 0.0
 * @max 50.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOR_MAX, 20.0f);

/**
 * Minimun deflection angle of right aileron
 *
 * @unit deg
 * @min -50.0
 * @max 0.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOR_MIN, -20.0f);

/**
 * Maximum deflection angle of left aileron
 *
 * @unit deg
 * @min 0.0
 * @max 50.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOL_MAX, 20.0f);

/**
 * Minimun deflection angle of left aileron
 *
 * @unit deg
 * @min -50.0
 * @max 0.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOL_MIN, -20.0f);

/**
 * aileron identification.>0 pitch; <0 roll
 *
 * @unit deg
 * @min -30.0
 * @max 30.0
 * @decimal 1
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_SERVOR_CON, 0.0f);


/**
 * Using aerodynamic models to compensate for aerodynamic forces when force map mode is feedforward compensation mode
 *
 * @value 0 Ignore aero force in offboard mode
 * @value 1 Compensate aero force in offboard mode
 * @group LW Model Configuration
 */
PARAM_DEFINE_INT32(LW_AERO_COM, 0);

/**
 * Drag coefficient in aerodynamic models
 *
 * @min 0.01
 * @max 2.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_AERO_C0, 0.1f);

/**
 * Lift coefficient in aerodynamic models
 *
 * @min 0.0
 * @max 3.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_AERO_C1, 1.5f);

/**
 * Side drag coefficient in aerodynamic models
 *
 * @min 0.01
 * @max 2.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_AERO_Y0, 0.3f);

/**
 * Roll torque coefficient of rudder surface
 *
 * @min -2.0
 * @max 2.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_LBCL_D, 0.0f);

/**
 * Pitch torque coefficient of rudder surface
 *
 * @min -2.0
 * @max 2.0
 * @decimal 2
 * @group LW Model Configuration
 */
PARAM_DEFINE_FLOAT(LW_LBCM_D, -0.0f);

//******************************************************************************************************************//
//**********************************************Disturdance Estimation**********************************************//
//******************************************************************************************************************//
/**
 * Alpha controller enable
 *
 * 1: new controller, 0: original controller
 *
 * @min 0
 * @max 1
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_ALPHA_CTRL_EN, 0.0f);

/**
 * Enable wind estimation
 *
 * @value 0 Disable
 * @value 1 Enable
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_INT32(WE_EN, 0);


/**
 * Roll/pitch alpha tracking bandwidth
 *
 * @unit rad/s
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_K_ALPHA_XY, 20.0f);

/**
 * Yaw alpha tracking bandwidth
 *
 * @unit rad/s
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_K_ALPHA_Z, 10.0f);

/**
 * Roll/pitch alpha dot desired low-pass cutoff frequency
 *
 * @unit rad/s
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_ADOT_XY_FZ, 100.0f);

/**
 * Yaw alpha dot desired low-pass cutoff frequency
 *
 * @unit rad/s
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_ADOT_Z_FZ, 30.0f);


/**
 * Enable disturdance torque estimation
 *
 * @min 0
 * @max 1.0
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_EN, 0.0f);

/**
 * Enable disturdance force estimation
 *
 * @min 0
 * @max 1.0
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_EN, 0.0f);

/**
 * Filter bandwidth for xy disturdance torque estimation
 *
 * 2*wn*sigma for angular error, wn^2 for attitude error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_WNXY, 50.0f);

/**
 * Filter bandwidth for z disturdance torque estimation
 *
 * 2*wn*sigma for angular error, wn^2 for attitude error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_WNZ, 20.0f);

/**
 * Filter damping for xy disturdance torque estimation
 *
 * 2*wn*sigma for angular error, wn^2 for attitude error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_SIGXY, 1.0f);

/**
 * Filter damping for z disturdance torque estimation
 *
 * 2*wn*sigma for angular error, wn^2 for attitude error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_SIGZ, 1.0f);

/**
 * Integral coefficient for xy disturdance torque estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_LAMXY, 1.0f);

/**
 * Integral coefficient for z disturdance torque estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_LAMZ, 1.0f);

/**
 * Omega filter cutoff frequency
 *
 * @min 0.0
 * @max 500.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_WBCF, 0.0f);

/**
 * XY Disturbance torque filtering coefficient
 *
 * @unit Hz
 * @min 0.0
 * @max 500.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_CFXY, 0.0f);

/**
 * Z Disturbance torque filtering coefficient
 *
 * @unit Hz
 * @min 0.0
 * @max 500.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_CFZ, 0.0f);

/**
 * Filtering coefficient of thrust in disturbance torque estimation
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_TEST_TDT, 0.06f);

/**
 * Filter bandwidth for xy disturdance force estimation
 *
 * 2*wn*sigma for velocity error, wn^2 for position error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_WNXY, 2.0f);

/**
 * Filter bandwidth for z disturdance force estimation
 *
 * 2*wn*sigma for velocity error, wn^2 for position error
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_WNZ, 1.0f);

/**
 * Filter damping for xy disturdance force estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_SIGXY, 20.0f);

/**
 * Filter damping for z disturdance force estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_SIGZ, 20.0f);

/**
 * Integral coefficient for xy disturdance force estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_LAMXY, 1.0f);

/**
 * Integral coefficient for z disturdance force estimation
 *
 * @min 0.1
 * @max 100.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_LAMZ, 1.0f);

/**
 * Disturbance compensation coefficient
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_P, 1.0f);

/**
 * Disturbance force filtering coefficient
 *
 * @unit Hz
 * @min 0.0
 * @max 500.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_CF, 1.0f);

/**
 * Filtering coefficient of thrust in disturbance force estimation
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Disturdance Estimation
 */
PARAM_DEFINE_FLOAT(LW_FEST_TDT, 0.06f);

//******************************************************************************************************************//
//******************************************************Mixer*******************************************************//
//******************************************************************************************************************//

/**
 * Enable aileron for control
 *
 * @min 0.0
 * @max 1.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_USE_SER, 0.0f);

/**
 * The proportion of torque error to the energy consumption of the actuator
 *
 * The larger the value, the more you want to reduce torque error and pay less attention to energy consumption
 *
 * @min 0.0
 * @max 1000000.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_MIXER_GAM, 500.0f);

/**
 * Weight for desired thrust
 *
 * @min 1.0
 * @max 100.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_FZ, 20.0f);

/**
 * Weight for desired x-axis torque
 *
 * @min 1.0
 * @max 100.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_TAOX, 50.0f);

/**
 * Weight for desired y-axis torque
 *
 * @min 1.0
 * @max 100.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_TAOY, 50.0f);

/**
 * Weight for desired z-axis torque
 *
 * @min 1.0
 * @max 100.0
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_TAOZ, 15.0f);

/**
 * Weight for aileron at low speeds
 *
 * @min 0.0
 * @max 1000000.0
 * @decimal 2
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_SER, 1000.0f);

/**
 * Weight for aileron at high speeds
 *
 * @min 0.0
 * @max 100.0
 * @decimal 2
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_W_SERH, 10.0f);

/**
 * Output low-pass filtering cut-off frequency of servo
 *
 * @unit Hz
 * @min 0.0
 * @max 100.0
 * @decimal 2
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_SER_LP, 1.0f);

/**
 * Minimum thrust limit for one motor (percentage of minimum thrust to hovering thrust)
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_THR_LOW, 0.05f);

/**
 * Motor thrust change rate limit (percentage of thrust change to hovering thrust in a control cycle)
 *
 * @min 0.0
 * @max 0.5
 * @decimal 1
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_DTHR, 0.3f);

/**
 * Servo change rate limit (percentage of servo change to maximum servo rate in a control cycle)
 *
 * @min 0.0
 * @max 0.5
 * @decimal 1
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_DSER, 0.2f);

/**
 * Minimum airspeed when starting to use the control surface (if enable surface control)
 *
 * @min 0.0
 * @max 10.0
 * @decimal 1
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_VA_TMIN, 6.0f);

/**
 * Exceeding this airspeed will not limit the use of the control surface (if enable surface control)
 *
 * @min 3.0
 * @max 20.0
 * @decimal 1
 * @group LW Mixer
 */
PARAM_DEFINE_FLOAT(LW_VA_TMAX, 13.0f);

//******************************************************************************************************************//
//*************************************************Attitude Control*************************************************//
//******************************************************************************************************************//
/**
 * Enable coordinate turn control
 *
 * @min 0.0
 * @max 1.0
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_COOR_TURN, 0.0f);

/**
 * Throttle for hovering, only used in attitude control mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_THR_HOVER, 0.186f);

/**
 * Maximum roll angle, only used in attitude control mode
 *
 * @unit deg
 * @min 10.0
 * @max 45.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_ROLL_MAX, 30.0f);

/**
 * Maximum pitch angle, only used in attitude control mode
 *
 * @unit deg
 * @min 10.0
 * @max 45.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_PITCH_MAX, 30.0f);

/**
 * Maximum y-axis angular rate
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_MAX, 70.0f);

/**
 * Maximum y-axis angular rate in decelerating phase
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_DE_MAX, 70.0f);

/**
 * Maximum x-axis angular rate
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_MAX, 70.0f);

/**
 * Maximum z-axis angular rate
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_MAX, 70.0f);

/**
 * Reference yaw angular rate low-pass filter cutoff frequence
 *
 * @reboot_required true
 * @unit Hz
 * @min 0
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZSP_CF, 2.0f);


/**
 * Maximum x-axis angular rate setpoint, only used in acro mode
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_ACRO_P_MAX, 720.0f);

/**
 * Maximum y-axis angular rate setpoint, only used in acro mode
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_ACRO_R_MAX, 360.0f);

/**
 * Maximum z-axis angular rate setpoint, only used in acro mode
 *
 * @unit deg/s
 * @min 10.0
 * @max 1000.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_ACRO_Y_MAX, 100.0f);

/**
 * Pitch P gain
 *
 * Pitch proportional gain, i.e. desired angular speed in rad/s for error 1 rad.
 *
 * @min 0.0
 * @max 12
 * @decimal 2
 * @increment 0.1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_PITCH_P, 3.5f);

/**
 * Pitch rate P gain
 *
 * Pitch rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_P, 15.0f);

/**
 * Pitch rate P gain at high airspeed
 *
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_P2, 12.0f);

/**
 * Pitch rate I gain
 *
 * Pitch rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_I, 3.0f);

/**
 * Pitch rate D gain
 *
 * Pitch rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_D, 0.00f);

/**
 * Filter coefficients for roll rate error differentiation
 *
 *
 * @min 1.0
 * @max 10.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_N, 1.0f);

/**
 * Roll P gain
 *
 * Roll proportional gain, i.e. desired angular speed in rad/s for error 1 rad.
 *
 * @min 0.0
 * @max 12
 * @decimal 2
 * @increment 0.1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_ROLL_P, 3.5f);

/**
 * Roll rate P gain
 *
 * Roll rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_P, 15.0f);

/**
 * Roll rate P gain at high airspeed
 *
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_P2, 12.0f);

/**
 * Roll rate I gain
 *
 * Roll rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_I, 3.0f);

/**
 * Roll rate D gain
 *
 * Roll rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_D, 0.00f);

/**
 * Filter coefficients for roll rate error differentiation
 *
 *
 * @min 1.0
 * @max 10.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_N, 1.0f);

/**
 * Yaw P gain
 *
 * Yaw proportional gain, i.e. desired angular speed in rad/s for error 1 rad.
 *
 * @min 0.0
 * @max 12
 * @decimal 2
 * @increment 0.1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_YAW_P, 2.0f);

/**
 * Yaw rate P gain
 *
 * Yaw rate proportional gain, i.e. control output for angular speed error 1 rad/s.
 *
 * @min 0.1
 * @max 100.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_P, 10.0f);

/**
 * Yaw rate I gain
 *
 * Yaw rate integral gain. Can be set to compensate static thrust difference or gravity center offset.
 *
 * @min 0.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_I, 2.0f);

/**
 * Yaw rate D gain
 *
 * Yaw rate differential gain. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_D, 0.0f);

/**
 * Filter coefficients for yaw rate error differentiation
 *
 *
 * @min 1.0
 * @max 10.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_N, 1.0f);

/**
 * Roll rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WX_FF, 1.0f);

/**
 * Pitch rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WY_FF, 1.0f);

/**
 * Yaw rate feedforward
 *
 * Improves tracking performance.
 *
 * @min 0.0
 * @max 1.0
 * @decimal 1
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_WZ_FF, 1.0f);

/**
 *
 * The low-pass filtering cutoff frequency of the ideal differentiator. When fz=0, x_ Out=x_ In.
 *
 * @min 0.0
 * @max 200.0
 * @unit Hz
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_TAUXYSP_FZ, 0.0f);

/**
 *
 * The low-pass filtering cutoff frequency of the ideal differentiator. When fz=0, x_ Out=x_ In.
 *
 * @min 0.0
 * @max 200.0
 * @unit Hz
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_TAUZSP_FZ, 10.0f);

/**
 * Prediction time of torque control command for xy axis Smith predictor
 *
 * Prediction time of torque control command for Smith predictor. When Tm=0, the low-pass filtering with a cutoff frequency of fz is performed on x_in.
 *
 * @min 0.0
 * @max 1.0
 * @unit s
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_TAUXYSP_TM, 0.06f);

/**
 * Prediction time of torque control command for z axis Smith predictor
 *
 * Prediction time of torque control command for Smith predictor. When Tm=0, the low-pass filtering with a cutoff frequency of fz is performed on x_in.
 *
 * @min 0.0
 * @max 1.0
 * @unit s
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_TAUZSP_TM, 0.06f);

/**
 * Rocker center dead zone for roll setpoint
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_ROLL_DZ, 0.05f);

/**
 * Rocker center dead zone for pitch setpoint
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_PITCH_DZ, 0.05f);

/**
 * Rocker center dead zone for roll rate setpoint
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_WX_DZ, 0.01f);

/**
 * Rocker center dead zone for pitch rate setpoint
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_WY_DZ, 0.01f);

/**
 * Rocker center dead zone for yaw rate setpoint
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Attitude Control
 */
PARAM_DEFINE_FLOAT(LW_MAN_WZ_DZ, 0.1f);

//******************************************************************************************************************//
//*************************************************Position Control*************************************************//
//******************************************************************************************************************//
/**
 * The mapping method from expected force to attitude
 *
 * 1: Known aerodynamic model, direct compensation;
 * 2: Known the aerodynamic model, use differential flatness to solve;
 * 3: Unknown aerodynamic model
 *
 * @value 0 Feedforward compensation (Only in offboard)
 * @value 1 Differential flatness solver
 * @value 2 Ignore aero force
 * @group LW Position Control
 */
PARAM_DEFINE_INT32(LW_F_MAP, 1);

/**
 * Specify yaw behavior in force allocation
 *
 * @value 0 Tangent (Face Forward)
 * @value 1 Rc controlled
 * @group LW Position Control
 */
PARAM_DEFINE_INT32(LW_YAW_MODE, 1);

/**
 * speed threshold
 *
 * When using YAW MODE 2, the speed threshold is specified when the yaw direction is consistent with the speed direction
 *
 * @min 0
 * @max 100
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_YAWMODE2_VA, 5.0f);

/**
 * Active abandonment of yaw angle control
 *
 * When the mixer cannot meet the expected yaw torque, the position control actively abandons the control of yaw angle
 *
 * @min 0
 * @max 1
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_REX_YAW, 0.0f);

/**
 * position control modes
 *
 * @value 0 position mode (with lateral/backward restrictions)
 * @value 1 Acceleration mode (with lateral/backward restrictions)
 * @value 2 Position mode (omnidirectional unrestricted, multirotor mode)
 * @value 3 Acceleration mode (omnidirectional unrestricted, multirotor mode)
 * @value 4 Altitude cruise mode (with lateral/backward restrictions, lifting-wing mode)
 * @group LW Position Control
 */
PARAM_DEFINE_INT32(LW_POS_MODE, 2);

/**
 * Maximum tilt angle in air
 *
 * Limits maximum tilt in POSCTRL modes during flight.
 *
 * @unit deg
 * @min 10.0
 * @max 90.0
 * @decimal 1
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_TILTMAX_AIR, 30.0f);

/**
 * Maximum horizontal velocity
 *
 * Maximum horizontal velocity and maximum velocity setpoint on x-axies in POSCTRL mode. If higher speeds
 * are commanded they will be capped to this velocity.
 *
 * @unit m/s
 * @min 0.0
 * @max 30.0
 * @increment 1
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_MAX, 5.0f);

/**
 * Maximum velocity setpoint on y-axies for manual control mode
 *
 * If velocity setpoint larger than LW_Y_VEL_MAX is set, then
 * the setpoint will be capped to LW_Y_VEL_MAX
 *
 * @unit m/s
 * @min 1.0
 * @max 20.0
 * @increment 1
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_MAX, 5.0f);

/**
 * Cutoff frequency of velocity command in position control mode
 *
 * @reboot_required true
 * @min 0.0
 * @max 50.0
 * @increment 0.01
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_XY_VELSP_CF, 2.0f);

/**
 * Cutoff frequency of velocity command in position control mode
 *
 * @reboot_required true
 * @unit Hz
 * @min 0.0
 * @max 10.0
 * @increment 0.01
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VELSP_CF, 2.0f);


/**
 * Maximum ascent velocity
 *
 * Ascent velocity in manual modes
 *
 * @unit m/s
 * @min 0.5
 * @max 8.0
 * @increment 0.1
 * @decimal 1
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_MAX_UP, 3.0f);

/**
 * Maximum x-axis acceleration in position control modes
 *
 * @unit m/s^2
 * @min 2.0
 * @max 10.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_ACCEL_MAX, 4.0f);

/**
 * Minimum x-axis acceleration in position control modes
 *
 * @unit m/s^2
 * @min -10.0
 * @max -2.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_ACCEL_MIN, -4.0f);

/**
 * Maximum jerk limit on x-axis
 *
 * Limit the maximum jerk of the vehicle (how fast the acceleration can change).
 * A lower value leads to smoother vehicle motions, but it also limits its
 * agility (how fast it can change directions or break).
 *
 * Setting this to the maximum value essentially disables the limit.
 *
 * Note: This is only used in Pilot mode.
 *
 * @unit m/s^3
 * @min 0.5
 * @max 500.0
 * @increment 1
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_JERK_MAX, 10.0f);

/**
 * Maximum y-axis acceleration in velocity controlled modes
 *
 * @unit m/s^2
 * @min 2.0
 * @max 10.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_ACCEL_MAX, 4.0f);

/**
 * Minimum y-axis acceleration in velocity controlled modes
 *
 * @unit m/s^2
 * @min -10.0
 * @max -2.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_ACCEL_MIN, -4.0f);

/**
 * Maximum jerk limit on y-axis
 *
 * Limit the maximum jerk of the vehicle (how fast the acceleration can change).
 * A lower value leads to smoother vehicle motions, but it also limits its
 * agility (how fast it can change directions or break).
 *
 * Setting this to the maximum value essentially disables the limit.
 *
 * Note: This is only used in Pilot mode.
 *
 * @unit m/s^3
 * @min 0.5
 * @max 500.0
 * @increment 1
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_JERK_MAX, 10.0f);

/**
 * Minimum z-axis acceleration (The norm is the maximum upward acceleration) in velocity controlled modes
 *
 * @unit m/s^2
 * @min -20.0
 * @max -2.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_ACCEL_LOW, -13.1f);

/**
 * Maximum z-axis acceleration (The norm is the maximum downward acceleration) in velocity controlled modes
 *
 * @unit m/s^2
 * @min 2.0
 * @max 10.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_ACCEL_UP, 6.8f);

/**
 * Maximum jerk limit on z-axis
 *
 * Limit the maximum jerk of the vehicle (how fast the acceleration can change).
 * A lower value leads to smoother vehicle motions, but it also limits its
 * agility (how fast it can change directions or break).
 *
 * Setting this to the maximum value essentially disables the limit.
 *
 * Note: This is only used in Pilot mode.
 *
 * @unit m/s^3
 * @min 0.5
 * @max 500.0
 * @increment 1
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_JERK_MAX, 10.0f);

/**
 * Feedforward gain for x-axis velocity in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_FF, 1.0f);

/**
 * Feedforward gain for y-axis velocity in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_FF, 1.0f);

/**
 * Feedforward gain for z-axis velocity in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_FF, 1.0f);

/**
 * Proportional gain for x-axis position error
 *
 * @min 0.0
 * @max 2.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_P, 1.0f);

/**
 * Proportional gain for x-axis velocity error in heading coordinate in Pilot mode, in NED coordinate in Pilot mode.
 *
 * defined as correction acceleration in m/s^2 per m/s velocity error
 *
 * @min 1.0
 * @max 5.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_P, 2.0f);

/**
 * Integral gain for x-axis velocity error
 *
 * defined as correction acceleration in m/s^2 per m velocity integral
 * Non-zero value allows to eliminate steady state errors in the presence of disturbances like wind.
 *
 * @min 0.0
 * @max 60.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_I, 0.5f);

/**
 * Differential gain for x-axis velocity error.
 *
 * defined as correction acceleration in m/s^2 per m/s^2 velocity derivative
 *
 * @min 0.0
 * @max 2.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_D, 0.0f);

/**
 * Low pass filter cut freq. for numerical velocity derivative
 *
 * @unit Hz
 * @min 0.0
 * @max 10
 * @decimal 2
 * @reboot_required true
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VELD_FZ, 5.0f);

/**
 * Low pass filter cut freq. for numerical velocity derivative
 *
 * @unit Hz
 * @min 0.0
 * @max 10
 * @decimal 2
 * @reboot_required true
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VELD_FZ, 5.0f);

/**
 * Low pass filter cut freq. for numerical velocity derivative
 *
 * @unit Hz
 * @min 0.0
 * @max 10
 * @decimal 2
 * @reboot_required true
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VELD_FZ, 5.0f);



/**
 * Filter coefficients for x-axis velocity error differentiation
 *
 * @min 0.0
 * @max 100.0
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_N, 1.0f);

/**
 * Proportional gain for y-axis position error
 *
 * @min 0.0
 * @max 2.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_P, 1.0f);

/**
 * Proportional gain for y-axis velocity error in heading coordinate in Pilot mode, in NED coordinate in Pilot mode.
 *
 * defined as correction acceleration in m/s^2 per m/s velocity error
 *
 * @min 1.0
 * @max 5.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_P, 2.0f);

/**
 * Integral gain for y-axis velocity error
 *
 * defined as correction acceleration in m/s^2 per m velocity integral
 * Non-zero value allows to eliminate steady state errors in the presence of disturbances like wind.
 *
 * @min 0.0
 * @max 60.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_I, 0.5f);

/**
 * Differential gain for y-axis velocity error.
 *
 * defined as correction acceleration in m/s^2 per m/s^2 velocity derivative
 *
 * @min 0.0
 * @max 2.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_D, 0.0f);

/**
 * Filter coefficients for y-axis velocity error differentiation
 *
 * @min 0.0
 * @max 100.0
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_N, 5.0f);

/**
 * Proportional gain for vertical position error
 *
 * @min 0.0
 * @max 3.5
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_P, 1.0f);
/**
 * Proportional gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m/s velocity error
 *
 * @min 1.0
 * @max 15.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_P, 4.0f);

/**
 * Integral gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m velocity integral
 *
 *
 * @min 0.0
 * @max 3.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_I, 0.5f);

/**
 * Differential gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m/s^2 velocity derivative
 *
 * @min 0.0
 * @max 2.0
 * @decimal 3
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_D, 0.0f);

/**
 * Filter coefficients for vertical velocity error differentiation
 *
 *
 * @min 1.0
 * @max 100.0
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_N, 1.0f);

/**
 * Feedforward gain for x-axis acceleration in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_ACCX_FF, 1.0f);

/**
 * Feedforward gain for y-axis acceleration in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_ACCY_FF, 1.0f);

/**
 * Feedforward gain for z-axis acceleration in Offboard mode
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_ACCZ_FF, 1.0f);

/**
 * The cut-off frequency of low-pass filtering for acc xy feedback reference
 *
 * The low-pass filtering cutoff frequency of the acc xy feedback reference. When fz=0, x_ Out=x_ In.
 *
 * @min 0.0
 * @max 200.0
 * @unit Hz
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_ACCXYSP_FZ, 0.0f);

/**
 * The cut-off frequency of low-pass filtering for acc z feedback reference
 *
 * The low-pass filtering cutoff frequency of the acc z feedback reference. When fz=0, x_ Out=x_ In.
 *
 * @min 0.0
 * @max 200.0
 * @unit Hz
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_ACCZSP_FZ, 0.0f);



/**
 * Rocker center dead zone for x-axis velocity
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_X_VEL_DZ, 0.1f);

/**
 * Rocker center dead zone for y-axis velocity
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Y_VEL_DZ, 0.1f);

/**
 * Rocker center dead zone for x-axis velocity
 *
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @group LW Position Control
 */
PARAM_DEFINE_FLOAT(LW_Z_VEL_DZ, 0.1f);


//******************************************************************************************************************//
//*****************************************************Land Detector*****************************************************//
//******************************************************************************************************************//
/**
 * Minimum thrust as a percentage of hovering thrust
 * 
 * Determines the idle thrust baseline used to compute low throttle thresholds. 
 * The algorithm computes a 30% margin over idle thrust for ground contact 
 * detection, and a tighter 20% margin for the "maybe landed" state detection.
 *
 * @unit norm
 * @min 0.05
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LW_THR_MIN, 0.1f);

/**
 * Total land detection trigger time
 *
 * Total time it takes to go through all three land detection stages:
 * ground contact, maybe landed, landed
 * when all necessary conditions are constantly met.
 *
 * @unit s
 * @min 0.1
 * @max 10.0
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_TRIG_TIME, 1.0f);

/**
 * Minimum ground force as a percentage of gravity
 * 
 * The ground force estimated by the force disturbance observer as a 
 * percentage of gravity is considered to be in contact with the ground 
 * and may land if it exceeds this value
 *
 * @unit norm
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @increment 0.1
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_GND_F, 0.5f);


/**
 * Maximum vertical velocity for landing
 *
 * The maximum vertical descent velocity (m/s) allowed to satisfy the ground 
 * contact condition. Once the "landed" state is confirmed, this threshold is 
 * internally multiplied by 2.5 to prevent false un-landing.
 *
 * @unit m/s
 * @min 0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_Z_VEL_MAX, 0.25f);

/**
 * Maximum horizontal velocity for landing
 *
 * The maximum horizontal velocity norm (XY-axis) allowed to satisfy the 
 * ground contact condition. Exceeding this means the vehicle is translating 
 * horizontally and should not trigger a landing state.
 *
 * @unit m/s
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_XY_VEL_MAX, 1.5f);

/**
 * Maximum rotational velocity for landing
 *
 * The maximum allowed angular velocity norm (pitch and roll axes) to trigger 
 * the "maybe landed" state. Once the vehicle enters the "landed" state, 
 * this threshold is multiplied by 2.5 to prevent ground bumps from resetting it.
 *
 * @unit deg/s
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ROT_MAX, 20.0f);

/**
 * Maximum altitude for multicopters
 *
 * The system will obey this limit as a
 * hard altitude limit. This setting will
 * be consolidated with the GF_MAX_VER_DIST
 * parameter.
 * A negative value indicates no altitude limitation.
 *
 * @unit m
 * @min -1
 * @max 10000
 * @decimal 2
 * @group Land Detector
 *
 */
PARAM_DEFINE_FLOAT(LNDMC_ALT_MAX, -1.0f);

/**
 * Ground effect altitude
 *
 * The height above ground below which ground effect creates barometric altitude errors.
 * A negative value indicates no ground effect.
 *
 * @unit m
 * @min -1
 * @decimal 2
 * @group Land Detector
 *
 */
PARAM_DEFINE_FLOAT(LNDMC_ALT_GND, 2.f);


/**
 * Auto landing blocked detector history window
 *
 * Time window used to compare expected and actual vertical position,
 * velocity, and support force changes.
 *
 * @unit s
 * @min 0.05
 * @max 1.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_WIN, 0.2f);

/**
 * Auto landing near-ground height
 *
 * Distance sensor threshold below which automatic landing blocked
 * detection is allowed.
 *
 * @unit m
 * @min 0.05
 * @max 2.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_ALT, 0.3f);

/**
 * Auto landing minimum time fallback
 *
 * If distance sensor data is unavailable, automatic landing blocked
 * detection is allowed after this much time in landing mode.
 *
 * @unit s
 * @min 0.0
 * @max 10.0
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_TMIN, 1.0f);

/**
 * Auto landing minimum downward velocity setpoint
 *
 * Minimum downward vertical velocity reference required to treat
 * automatic landing as actively commanding descent.
 *
 * @unit m/s
 * @min 0.0
 * @max 2.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_VZSP, 0.1f);

/**
 * Auto landing minimum downward position reference increment
 *
 * Minimum positive NED z reference change over the detector window
 * required to treat automatic landing as actively commanding descent.
 *
 * @unit m
 * @min 0.0
 * @max 1.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_ZSP, 0.1f);

/**
 * Auto landing blocked vertical velocity threshold
 *
 * Actual vertical velocity must stay below this absolute value to
 * indicate descent is blocked.
 *
 * @unit m/s
 * @min 0.0
 * @max 1.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_VZ, 0.15f);

/**
 * Auto landing blocked vertical velocity delta
 *
 * Maximum actual vertical velocity change over the detector window
 * for the descent-blocked condition.
 *
 * @unit m/s
 * @min 0.0
 * @max 2.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_DVZ, 0.3f);

/**
 * Auto landing blocked vertical position delta
 *
 * Maximum actual NED z change over the detector window for the
 * descent-blocked condition.
 *
 * @unit m
 * @min 0.0
 * @max 1.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_DZ, 0.5f);

/**
 * Auto landing support force threshold
 *
 * Upward support acceleration threshold as a percentage of gravity.
 *
 * @unit %
 * @min 0.0
 * @max 200.0
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_SUP, 30.0f);

/**
 * Auto landing support force increase threshold
 *
 * Upward support acceleration increase over the detector window as a
 * percentage of gravity.
 *
 * @unit %
 * @min 0.0
 * @max 200.0
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_DSUP, 10.0f);

/**
 * Auto landing blocked detection confirmation time
 *
 * Time for which all auto landing blocked conditions must hold before
 * entering idle protection.
 *
 * @unit s
 * @min 0.02
 * @max 2.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_T, 0.2f);

/**
 * Auto landing blocked timer decay gain
 *
 * Rate at which the confirmation timer decays when blocked conditions
 * are not met.
 *
 * @min 0.0
 * @max 10.0
 * @decimal 1
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_DEC, 2.0f);

/**
 * Auto landing upward velocity abort threshold
 *
 * If the vertical velocity reference is more upward than this NED value,
 * the auto landing blocked detector aborts.
 *
 * @unit m/s
 * @min -2.0
 * @max 0.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_VZUP, -0.1f);

/**
 * Auto landing upward position reference abort increment
 *
 * If the NED z reference decreases by more than this value over the
 * detector window, the auto landing blocked detector aborts.
 *
 * @unit m
 * @min 0.0
 * @max 1.0
 * @decimal 2
 *
 * @group Land Detector
 */
PARAM_DEFINE_FLOAT(LNDMC_ABL_ZUP, 0.1f);



//******************************************************************************************************************//
//*****************************************************offboard*****************************************************//
//******************************************************************************************************************//
/**
 * Turnoff yaw control when Va>6m/s.
 *
 * @min 0.0
 * @max 1.0
 * @group LW Offboard
 */
PARAM_DEFINE_FLOAT(LW_OFFYAW, 0.0f);

/**
 * Choose between different trajectories
 *
 * @min 0.0
 * @max 2.0
 * @group LW Offboard
 */
PARAM_DEFINE_FLOAT(LW_OB_TYPE, 0.0f);

/**
 * Radius for circular trajectory; desired heading(rad) for linear trajectory
 *
 * @min -5
 * @max 500.0
 * @decimal 2
 * @group LW Offboard
 */
PARAM_DEFINE_FLOAT(LW_OB_CIRCLE_R, 2.8f);

/**
 * Desired maximum velocity for trajectory
 *
 * @min 0.0
 * @max 30.0
 * @decimal 2
 * @group LW Offboard
 */
PARAM_DEFINE_FLOAT(LW_OB_CIRCLE_V, 6.0f);

/**
 * Angular velocity(rad/s) for circular trajectory and time to maintain(s) the maximum velocity for linear trajectory
 *
 * @min 0.0
 * @decimal 2
 * @group LW Offboard
 */
PARAM_DEFINE_FLOAT(LW_OB_CIRCLE_W, 3.0f);


///



/**
 * Minimum collective thrust in auto thrust control
 *
 * It's recommended to set it > 0 to avoid free fall with zero thrust.
 * Note: Without airmode zero thrust leads to zero roll/pitch control authority. (see MC_AIRMODE)
 *
 * @unit norm
 * @min 0.05
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_THR_MIN, 0.12f);

/**
 * Hover thrust
 *
 * Vertical thrust required to hover.
 * This value is mapped to center stick for manual throttle control.
 * With this value set to the thrust required to hover, transition
 * from manual to Altitude or Position mode while hovering will occur with the
 * throttle stick near center, which is then interpreted as (near)
 * zero demand for vertical speed.
 *
 * This parameter is also important for the landing detection to work correctly.
 *
 * @unit norm
 * @min 0.1
 * @max 0.8
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_THR_HOVER, 0.5f);

/**
 * Hover thrust source selector
 *
 * Set false to use the fixed parameter MPC_THR_HOVER
 * Set true to use the value computed by the hover thrust estimator
 *
 * @boolean
 * @group Multicopter Position Control
 */
PARAM_DEFINE_INT32(MPC_USE_HTE, 1);

/**
 * Thrust curve in Manual Mode
 *
 * This parameter defines how the throttle stick input is mapped to commanded thrust
 * in Manual/Stabilized flight mode.
 *
 * In case the default is used ('Rescale to hover thrust'), the stick input is linearly
 * rescaled, such that a centered stick corresponds to the hover throttle (see MPC_THR_HOVER).
 *
 * Select 'No Rescale' to directly map the stick 1:1 to the output. This can be useful
 * in case the hover thrust is very low and the default would lead to too much distortion
 * (e.g. if hover thrust is set to 20%, 80% of the upper thrust range is squeezed into the
 * upper half of the stick range).
 *
 * Note: In case MPC_THR_HOVER is set to 50%, the modes 0 and 1 are the same.
 *
 * @value 0 Rescale to hover thrust
 * @value 1 No Rescale
 * @group Multicopter Position Control
 */
PARAM_DEFINE_INT32(MPC_THR_CURVE, 0);

/**
 * Horizontal thrust margin
 *
 * Margin that is kept for horizontal control when prioritizing vertical thrust.
 * To avoid completely starving horizontal control with high vertical error.
 *
 * @unit norm
 * @min 0.0
 * @max 0.5
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_THR_XY_MARG, 0.3f);

/**
 * Maximum thrust in auto thrust control
 *
 * Limit max allowed thrust
 *
 * @unit norm
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_THR_MAX, 1.0f);

/**
 * Minimum manual thrust
 *
 * Minimum vertical thrust. It's recommended to set it > 0 to avoid free fall with zero thrust.
 * With MC_AIRMODE set to 1, this can safely be set to 0.
 *
 * @unit norm
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_MANTHR_MIN, 0.08f);

/**
 * Proportional gain for vertical position error
 *
 * @min 0.0
 * @max 1.5
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_P, 1.0f);

/**
 * Proportional gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m/s velocity error
 *
 * @min 2.0
 * @max 15.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_P_ACC, 4.0f);

/**
 * Integral gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m velocity integral
 *
 * Non zero value allows hovering thrust estimation on stabilized or autonomous takeoff.
 *
 * @min 0.2
 * @max 3.0
 * @decimal 3
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_I_ACC, 2.0f);

/**
 * Differential gain for vertical velocity error
 *
 * defined as correction acceleration in m/s^2 per m/s^2 velocity derivative
 *
 * @min 0.0
 * @max 2.0
 * @decimal 3
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_D_ACC, 0.0f);

/**
 * Automatic ascent velocity
 *
 * Ascent velocity in auto modes.
 * For manual modes and offboard, see MPC_Z_VEL_MAX_UP
 *
 * @unit m/s
 * @min 0.5
 * @max 8.0
 * @increment 0.1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_V_AUTO_UP, 3.f);

/**
 * Maximum ascent velocity
 *
 * Ascent velocity in manual modes and offboard.
 * For auto modes, see MPC_Z_V_AUTO_UP
 *
 * @unit m/s
 * @min 0.5
 * @max 8.0
 * @increment 0.1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_MAX_UP, 2.f);

/**
 * Automatic descent velocity
 *
 * Descent velocity in auto modes.
 * For manual modes and offboard, see MPC_Z_VEL_MAX_DN
 *
 * @unit m/s
 * @min 0.5
 * @max 4.0
 * @increment 0.1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_V_AUTO_DN, 1.5f);

/**
 * Maximum descent velocity
 *
 * Descent velocity in manual modes and offboard.
 * For auto modes, see MPC_Z_V_AUTO_DN
 *
 * @unit m/s
 * @min 0.5
 * @max 4.0
 * @increment 0.1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_MAX_DN, 1.5f);

/**
 * Proportional gain for horizontal position error
 *
 * @min 0.0
 * @max 2.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_P, 0.95f);

/**
 * Proportional gain for horizontal velocity error
 *
 * defined as correction acceleration in m/s^2 per m/s velocity error
 *
 * @min 1.2
 * @max 5.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_VEL_P_ACC, 1.8f);

/**
 * Integral gain for horizontal velocity error
 *
 * defined as correction acceleration in m/s^2 per m velocity integral
 * Non-zero value allows to eliminate steady state errors in the presence of disturbances like wind.
 *
 * @min 0.0
 * @max 60.0
 * @decimal 3
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_VEL_I_ACC, 0.4f);

/**
 * Differential gain for horizontal velocity error. Small values help reduce fast oscillations. If value is too big oscillations will appear again.
 *
 * defined as correction acceleration in m/s^2 per m/s^2 velocity derivative
 *
 * @min 0.1
 * @max 2.0
 * @decimal 3
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_VEL_D_ACC, 0.2f);

/**
 * Default horizontal cruise speed in mission
 *
 * Horizontal cruise speed used when flying autonomously. With MPC_MIS_SPD_MODE=0
 * this is ground speed (original behavior). With MPC_MIS_SPD_MODE=1 it is the default
 * cruising airspeed for normal multicopter AUTO Mission POSITION legs; the resulting
 * ground-speed constraint is still limited by MPC_XY_VEL_MAX.
 *
 * @unit m/s
 * @min 3.0
 * @max 20.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_CRUISE, 10.0f);


/**
 * Mission cruise speed interpretation for multicopter
 *
 * Selects how the horizontal speed requested by an AUTO Mission is interpreted.
 * Ground-speed mode keeps the original PX4 behavior. Airspeed mode interprets
 * the Mission speed as cruising airspeed on normal POSITION legs and converts it
 * to a ground-speed trajectory constraint using the estimated horizontal wind.
 * Takeoff, landing, loiter, off-track recovery and other non-cruise phases keep
 * ground-speed behavior. MPC_XY_VEL_MAX always limits ground speed.
 *
 * @min 0
 * @max 1
 * @value 0 Mission speed is ground speed
 * @value 1 Mission speed is cruising airspeed
 * @group Mission
 */
PARAM_DEFINE_INT32(MPC_MIS_SPD_MODE, 0);

/**
 * Maximum mission cruise airspeed
 *
 * Maximum commanded cruise airspeed when MPC_MIS_SPD_MODE is set to airspeed mode.
 * The Mission speed is limited to this value before wind compensation. The final
 * ground-speed trajectory constraint is limited separately by MPC_XY_VEL_MAX.
 * If the current wind makes the airspeed/ground-track constraints infeasible, the
 * first implementation keeps executing the Mission track and sends a warning.
 *
 * @unit m/s
 * @min 1.0
 * @max 40.0
 * @increment 0.5
 * @decimal 1
 * @group Mission
 */
PARAM_DEFINE_FLOAT(MPC_MIS_ASPD_MAX, 12.0f);

/**
 * Proportional gain for horizontal trajectory position error
 *
 * @min 0.1
 * @max 1.0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_TRAJ_P, 0.5f);

/**
 * Maximum horizontal error allowed by the trajectory generator
 *
 * The integration speed of the trajectory setpoint is linearly
 * reduced with the horizontal position tracking error. When the
 * error is above this parameter, the integration of the
 * trajectory is stopped to wait for the drone.
 *
 * This value can be adjusted depending on the tracking
 * capabilities of the vehicle.
 *
 * @min 0.1
 * @max 10.0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_ERR_MAX, 2.0f);

/**
 * Maximum horizontal velocity setpoint in Position mode
 *
 * If velocity setpoint larger than MPC_XY_VEL_MAX is set, then
 * the setpoint will be capped to MPC_XY_VEL_MAX
 *
 * The maximum sideways and backward speed can be set differently
 * using MPC_VEL_MAN_SIDE and MPC_VEL_MAN_BACK, respectively.
 *
 * @unit m/s
 * @min 3.0
 * @max 20.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_VEL_MANUAL, 10.0f);

/**
 * Maximum sideways velocity in Position mode
 *
 * If set to a negative value or larger than
 * MPC_VEL_MANUAL then MPC_VEL_MANUAL is used.
 *
 * @unit m/s
 * @min -1.0
 * @max 20.0
 * @increment 0.1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_VEL_MAN_SIDE, -1.0f);

/**
 * Maximum backward velocity in Position mode
 *
 * If set to a negative value or larger than
 * MPC_VEL_MANUAL then MPC_VEL_MANUAL is used.
 *
 * @unit m/s
 * @min -1.0
 * @max 20.0
 * @increment 0.1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_VEL_MAN_BACK, -1.0f);

/**
 * Maximum horizontal velocity
 *
 * Maximum horizontal velocity in AUTO mode. If higher speeds
 * are commanded in a mission they will be capped to this velocity.
 *
 * @unit m/s
 * @min 0.0
 * @max 20.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_VEL_MAX, 5.0f);

/**
 * Maximum tilt angle in air
 *
 * Limits maximum tilt in AUTO and POSCTRL modes during flight.
 *
 * @unit deg
 * @min 20.0
 * @max 89.0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_TILTMAX_AIR, 45.0f);

/**
 * Maximum tilt during landing
 *
 * Limits maximum tilt angle on landing.
 *
 * @unit deg
 * @min 10.0
 * @max 89.0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_TILTMAX_LND, 12.0f);

/**
 * Landing descend rate
 *
 * @unit m/s
 * @min 0.6
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_SPEED, 0.7f);

/**
 * Land crawl descend rate
 *
 * Used below MPC_LAND_ALT3 if distance sensor data is availabe.
 *
 * @unit m/s
 * @min 0.1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_CRWL, 0.3f);

/**
 * Enable user assisted descent for autonomous land routine
 *
 * When enabled, descent speed will be:
 * stick full up - 0
 * stick centered - MPC_LAND_SPEED
 * stick full down - 2 * MPC_LAND_SPEED
 *
 * Additionally, the vehicle can be yawed and moved laterally using the other sticks.
 * Manual override during auto modes has to be disabled to use this feature (see COM_RC_OVERRIDE).
 *
 * @min 0
 * @max 1
 * @value 0 Fixed descent speed of MPC_LAND_SPEED
 * @value 1 User assisted descent speed
 * @group Multicopter Position Control
 */
PARAM_DEFINE_INT32(MPC_LAND_RC_HELP, 0);

/**
 * User assisted landing radius
 *
 * When user assisted descent is enabled (see MPC_LAND_RC_HELP),
 * this parameter controls the maximum position adjustment
 * allowed from the original landing point.
 *
 * @unit m
 * @min 0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_RADIUS, 1000.f);

/**
 * Takeoff climb rate
 *
 * @unit m/s
 * @min 1
 * @max 5
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_TKO_SPEED, 1.5f);

/**
 * Maximal tilt angle in manual or altitude mode
 *
 * @unit deg
 * @min 0.0
 * @max 90.0
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_MAN_TILT_MAX, 35.0f);

/**
 * Max manual yaw rate
 *
 * @unit deg/s
 * @min 0.0
 * @max 400
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_MAN_Y_MAX, 150.0f);

/**
 * Manual yaw rate input filter time constant
 *
 * Setting this parameter to 0 disables the filter
 *
 * @unit s
 * @min 0.0
 * @max 5.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_MAN_Y_TAU, 0.08f);

/**
 * Deadzone of sticks where position hold is enabled
 *
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_HOLD_DZ, 0.1f);

/**
 * Maximum horizontal velocity for which position hold is enabled (use 0 to disable check)
 *
 * @unit m/s
 * @min 0.0
 * @max 3.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_HOLD_MAX_XY, 0.8f);

/**
 * Maximum vertical velocity for which position hold is enabled (use 0 to disable check)
 *
 * @unit m/s
 * @min 0.0
 * @max 3.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_HOLD_MAX_Z, 0.6f);

/**
 * Low pass filter cut freq. for numerical velocity derivative
 *
 * @unit Hz
 * @min 0.0
 * @max 10
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_VELD_LP, 5.0f);

/**
 * Maximum horizontal acceleration for auto mode and for manual mode
 *
 * MPC_POS_MODE
 * 1 just deceleration
 * 3 acceleration and deceleration
 * 4 just acceleration
 *
 * @unit m/s^2
 * @min 2.0
 * @max 15.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_ACC_HOR_MAX, 5.0f);

/**
 * Acceleration for auto and for manual
 *
 * Note: In manual, this parameter is only used in MPC_POS_MODE 4.
 *
 * @unit m/s^2
 * @min 2.0
 * @max 15.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */

PARAM_DEFINE_FLOAT(MPC_ACC_HOR, 3.0f);

/**
 * Maximum vertical acceleration in velocity controlled modes upward
 *
 * @unit m/s^2
 * @min 2.0
 * @max 15.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_ACC_UP_MAX, 4.0f);

/**
 * Maximum vertical acceleration in velocity controlled modes down
 *
 * @unit m/s^2
 * @min 2.0
 * @max 15.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_ACC_DOWN_MAX, 3.0f);

/**
 * Maximum jerk limit
 *
 * Limit the maximum jerk of the vehicle (how fast the acceleration can change).
 * A lower value leads to smoother vehicle motions, but it also limits its
 * agility (how fast it can change directions or break).
 *
 * Setting this to the maximum value essentially disables the limit.
 *
 * Note: This is only used when MPC_POS_MODE is set to a smoothing mode 3 or 4.
 *
 * @unit m/s^3
 * @min 0.5
 * @max 500.0
 * @increment 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_JERK_MAX, 8.0f);

/**
 * Jerk limit in auto mode
 *
 * Limit the maximum jerk of the vehicle (how fast the acceleration can change).
 * A lower value leads to smoother vehicle motions, but it also limits its
 * agility.
 *
 * @unit m/s^3
 * @min 1.0
 * @max 80.0
 * @increment 1
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_JERK_AUTO, 4.0f);

/**
 * Altitude control mode.
 *
 * Set to 0 to control height relative to the earth frame origin. This origin may move up and down in
 * flight due to sensor drift.
 * Set to 1 to control height relative to estimated distance to ground. The vehicle will move up and down
 * with terrain height variation. Requires a distance to ground sensor. The height controller will
 * revert to using height above origin if the distance to ground estimate becomes invalid as indicated
 * by the local_position.distance_bottom_valid message being false.
 * Set to 2 to control height relative to ground (requires a distance sensor) when stationary and relative
 * to earth frame origin when moving horizontally.
 * The speed threshold is controlled by the MPC_HOLD_MAX_XY parameter.
 *
 * @min 0
 * @max 2
 * @value 0 Altitude following
 * @value 1 Terrain following
 * @value 2 Terrain hold
 * @group Multicopter Position Control
 */
PARAM_DEFINE_INT32(MPC_ALT_MODE, 0);

/**
 * Manual position control stick exponential curve sensitivity
 *
 * The higher the value the less sensitivity the stick has around zero
 * while still reaching the maximum value with full stick deflection.
 *
 * 0 Purely linear input curve (default)
 * 1 Purely cubic input curve
 *
 * @min 0
 * @max 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_MAN_EXPO, 0.6f);

/**
 * Manual control stick vertical exponential curve
 *
 * The higher the value the less sensitivity the stick has around zero
 * while still reaching the maximum value with full stick deflection.
 *
 * 0 Purely linear input curve (default)
 * 1 Purely cubic input curve
 *
 * @min 0
 * @max 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_MAN_EXPO, 0.6f);

/**
 * Manual control stick yaw rotation exponential curve
 *
 * The higher the value the less sensitivity the stick has around zero
 * while still reaching the maximum value with full stick deflection.
 *
 * 0 Purely linear input curve (default)
 * 1 Purely cubic input curve
 *
 * @min 0
 * @max 1
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_YAW_EXPO, 0.6f);

/**
 * Max yaw rate in auto mode
 *
 * Limit the rate of change of the yaw setpoint in autonomous mode
 * to avoid large control output and mixer saturation.
 *
 * @unit deg/s
 * @min 0.0
 * @max 360.0
 * @decimal 1
 * @increment 5
 * @group Multicopter Attitude Control
 */
PARAM_DEFINE_FLOAT(MPC_YAWRAUTO_MAX, 30.0f);

/**
 * Altitude for 1. step of slow landing (descend)
 *
 * Below this altitude descending velocity gets limited to a value
 * between "MPC_Z_VEL_MAX_DN" (or "MPC_Z_V_AUTO_DN") and "MPC_LAND_SPEED"
 * Value needs to be higher than "MPC_LAND_ALT2"
 *
 * @unit m
 * @min 0
 * @max 122
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_ALT1, 10.0f);

/**
 * Altitude for 2. step of slow landing (landing)
 *
 * Below this altitude descending velocity gets
 * limited to "MPC_LAND_SPEED"
 * Value needs to be lower than "MPC_LAND_ALT1"
 *
 * @unit m
 * @min 0
 * @max 122
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_ALT2, 5.0f);

/**
 * Altitude for 3. step of slow landing
 *
 * Below this altitude descending velocity gets
 * limited to "MPC_LAND_CRWL", if LIDAR available.
 * No effect if LIDAR not available
 *
 * @unit m
 * @min 0
 * @max 122
 * @decimal 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_LAND_ALT3, 1.0f);

/**
 * Position control smooth takeoff ramp time constant
 *
 * Increasing this value will make automatic and manual takeoff slower.
 * If it's too slow the drone might scratch the ground and tip over.
 * A time constant of 0 disables the ramp
 *
 * @min 0
 * @max 5
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_TKO_RAMP_T, 3.0f);

/**
 * Smooth takeoff initial ramp release time
 *
 * This value should be smaller than MPC_TKO_RAMP_T.
 *
 * @unit s
 * @min 0.0
 * @max 5.0
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_TKO_RAMP_T0, 1.0f);

/**
 * Manual-Position control sub-mode
 *
 * The supported sub-modes are:
 * 0 Simple position control where sticks map directly to velocity setpoints
 *   without smoothing. Useful for velocity control tuning.
 * 3 Smooth position control with maximum acceleration and jerk limits based on
 *   jerk optimized trajectory generator (different algorithm than 1).
 * 4 Smooth position control where sticks map to acceleration and there's a virtual brake drag
 *
 * @value 0 Simple position control
 * @value 3 Smooth position control (Jerk optimized)
 * @value 4 Acceleration based input
 * @group Multicopter Position Control
 */
PARAM_DEFINE_INT32(MPC_POS_MODE, 4);

/**
 * Yaw mode.
 *
 * Specifies the heading in Auto.
 *
 * @min 0
 * @max 5
 * @value 0 towards waypoint
 * @value 1 towards home
 * @value 2 away from home
 * @value 3 along ground trajectory
 * @value 4 towards waypoint (yaw first)
 * @value 5 along desired airspeed direction
 * @group Mission
 */
PARAM_DEFINE_INT32(MPC_YAW_MODE, 0);

/**
 * Responsiveness
 *
 * Changes the overall responsiveness of the vehicle.
 * The higher the value, the faster the vehicle will react.
 *
 * If set to a value greater than zero, other parameters are automatically set (such as
 * the acceleration or jerk limits).
 * If set to a negative value, the existing individual parameters are used.
 *
 * @min -1
 * @max 1
 * @decimal 2
 * @increment 0.05
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(SYS_VEHICLE_RESP, -0.4f);

/**
 * Overall Horizontal Velocity Limit
 *
 * If set to a value greater than zero, other parameters are automatically set (such as
 * MPC_XY_VEL_MAX or MPC_VEL_MANUAL).
 * If set to a negative value, the existing individual parameters are used.
 *
 * @min -20
 * @max 20
 * @decimal 1
 * @increment 1
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_XY_VEL_ALL, -10.0f);

/**
 * Overall Vertical Velocity Limit
 *
 * If set to a value greater than zero, other parameters are automatically set (such as
 * MPC_Z_VEL_MAX_UP or MPC_LAND_SPEED).
 * If set to a negative value, the existing individual parameters are used.
 *
 * @min -3
 * @max 8
 * @decimal 1
 * @increment 0.5
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MPC_Z_VEL_ALL, -3.0f);


/**
 * Manual tilt input filter time constant
 *
 * Setting this parameter to 0 disables the filter
 *
 * @unit s
 * @min 0.0
 * @max 2.0
 * @decimal 2
 * @group Multicopter Position Control
 */
PARAM_DEFINE_FLOAT(MC_MAN_TILT_TAU, 0.0f);




