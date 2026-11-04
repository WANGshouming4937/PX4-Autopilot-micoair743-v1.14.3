/****************************************************************************
 *
 *   Copyright (c) 2017 PX4 Development Team. All rights reserved.
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
 * @file precland_params.c
 *
 * Parameters for precision landing.
 *
 * @author Nicolas de Palezieux (Sunflower Labs) <ndepal@gmail.com>
 */

/**
 * Landing Target Timeout
 *
 * Time after which the landing target is considered lost without any new measurements.
 *
 * @unit s
 * @min 0.0
 * @max 50
 * @decimal 1
 * @increment 0.5
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_BTOUT, 5.0f);

/**
 * Horizontal acceptance radius
 *
 * Start descending if closer above landing target than this.
 *
 * @unit m
 * @min 0.0
 * @max 10
 * @decimal 2
 * @increment 0.1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_HACC_RAD, 0.2f);

/**
 * Final approach altitude
 *
 * Allow final approach (without horizontal positioning) if losing landing target closer than this to the ground.
 * With PLD_XY_GATE enabled and MPC_LAND_ALT3 positive, PLD_XY_CMT_ALT
 * controls committed touchdown instead; this parameter retains legacy behavior.
 *
 * @unit m
 * @min 0.0
 * @max 10
 * @decimal 2
 * @increment 0.1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_FAPPR_ALT, 0.1f);

/**
 * Search altitude
 *
 * Altitude above home to which to climb when searching for the landing target.
 *
 * @unit m
 * @min 0.0
 * @max 100
 * @decimal 1
 * @increment 0.1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_SRCH_ALT, 10.0f);

/**
 * Search timeout
 *
 * Time allowed to search for the landing target before falling back to normal landing.
 *
 * @unit s
 * @min 0.0
 * @max 100
 * @decimal 1
 * @increment 0.1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_SRCH_TOUT, 10.0f);

/**
 * Maximum number of search attempts
 *
 * Maximum number of times to search for the landing target if it is lost during the precision landing.
 *
 * @min 0
 * @max 100
 * @group Precision Land
 */
PARAM_DEFINE_INT32(PLD_MAX_SRCH, 3);


/**
 * Terminal precision descent XY gate
 *
 * Below MPC_LAND_ALT3, hold altitude when XY alignment is not satisfactory.
 * After entering the terminal region the gate remains active until commitment.
 * MPC_LAND_ALT3 must be positive. Normal landing without PrecLand is unchanged.
 *
 * @boolean
 * @group Precision Land
 */
PARAM_DEFINE_INT32(PLD_XY_GATE, 1);

/**
 * XY error to pause terminal descent
 *
 * Euclidean horizontal distance from actual vehicle position to the valid
 * landing target estimate. Set greater than PLD_XY_GO.
 *
 * @unit m
 * @min 0.01
 * @max 2.0
 * @decimal 2
 * @increment 0.01
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_STOP, 0.05f);

/**
 * XY error to resume terminal descent
 *
 * Error must stay at or below this value, with speed below PLD_XY_VEL,
 * for PLD_XY_T seconds. Set lower than PLD_XY_STOP; larger values are capped.
 *
 * @unit m
 * @min 0.0
 * @max 2.0
 * @decimal 2
 * @increment 0.01
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_GO, 0.03f);

/**
 * Continuous XY alignment time before descent
 *
 * The timer resets on excessive error, excessive speed, stale/invalid target,
 * unavailable height, or a control-loop gap over 0.25 seconds.
 *
 * @unit s
 * @min 0.0
 * @max 5.0
 * @decimal 2
 * @increment 0.1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_T, 0.5f);

/**
 * Maximum XY speed to resume terminal descent
 *
 * @unit m/s
 * @min 0.01
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_VEL, 0.10f);

/**
 * Terminal XY hold timeout
 *
 * If a continuous pause exceeds this duration, abort precision descent and
 * hold position/altitude until the operator changes mode. Battery, RC and
 * other commander failsafes remain authoritative. Zero disables the timeout.
 * With visual yaw enabled, the initial shortest turn angle divided by the
 * effective yaw-rate limit is added to this budget (180 deg if yaw is unknown).
 *
 * @unit s
 * @min 0.0
 * @max 60.0
 * @decimal 1
 * @increment 1.0
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_TOUT, 10.0f);

/**
 * Distance for committed vertical touchdown
 *
 * Below this sensor-to-surface distance, latch the last commanded landing XY and
 * continue landing without marker updates, regardless of XY error/target loss.
 * This is NOT skid clearance. Set above the distance measured at touchdown
 * and below MPC_LAND_ALT3. The value is capped at MPC_LAND_ALT3.
 * Zero disables commitment. Only used when PLD_XY_GATE is enabled.
 *
 * @unit m
 * @min 0.0
 * @max 1.0
 * @decimal 2
 * @increment 0.01
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_XY_CMT_ALT, 0.20f);

/**
 * Vertical acceleration limit during a precision descent pause
 *
 * Uses the existing FlightTaskAuto trajectory to decelerate to zero vertical
 * speed, then hold the stopping altitude. The vehicle may descend by its
 * stopping distance; it is not commanded back to the pause-entry altitude.
 * Capped by MPC_ACC_UP_MAX and MPC_ACC_DOWN_MAX. Does not limit emergency
 * braking or change ordinary LAND, RTL return legs or manual flight.
 *
 * @unit m/s^2
 * @min 0.1
 * @max 2.0
 * @decimal 2
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_Z_ACC, 0.5f);

/**
 * Vertical jerk limit during a precision descent pause
 *
 * Smaller values soften the braking onset but increase stopping time and
 * distance. Capped by MPC_JERK_AUTO. Only applies to PrecLand altitude pauses.
 *
 * @unit m/s^3
 * @min 0.1
 * @max 8.0
 * @decimal 2
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_Z_JERK, 1.0f);

/**
 * Align terminal landing yaw to airport marker
 *
 * Requires PLD_XY_GATE enabled and positive MPC_LAND_ALT3. Alignment starts
 * in the same terminal altitude hold. Above it, native RTL heading is retained.
 * At PLD_XY_CMT_ALT actual yaw is latched along with XY, even if the marker is lost.
 *
 * @boolean
 * @group Precision Land
 */
PARAM_DEFINE_INT32(PLD_YAW_EN, 1);

/**
 * Landing yaw offset from airport forward
 *
 * Positive is clockwise in NED. Zero aligns aircraft forward with the sheet's
 * top edge direction, after per-marker forward-axis normalization in vision.
 *
 * @unit deg
 * @min -180
 * @max 180
 * @decimal 1
 * @increment 1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_YAW_OFF, 0.0f);

/**
 * Yaw error to pause terminal descent
 *
 * Actual aircraft heading versus the valid airport heading, not versus an
 * intermediate rate-limited yaw reference. Set above PLD_YAW_GO.
 *
 * @unit deg
 * @min 1
 * @max 90
 * @decimal 1
 * @increment 1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_YAW_STOP, 10.0f);

/**
 * Yaw error to resume terminal descent
 *
 * Yaw, XY, translation speed and yaw rate must all satisfy their resume
 * conditions continuously for PLD_XY_T. Capped at PLD_YAW_STOP.
 *
 * @unit deg
 * @min 0.1
 * @max 45
 * @decimal 1
 * @increment 1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_YAW_GO, 5.0f);

/**
 * Visual landing yaw reference rate limit
 *
 * The effective rate is the smaller of this and MPC_YAWRAUTO_MAX.
 *
 * @unit deg/s
 * @min 1
 * @max 90
 * @decimal 1
 * @increment 1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_YAW_RATE, 15.0f);

/**
 * Maximum actual yaw rate to resume landing
 *
 * Uses the filtered, wrapped local-position heading derivative. This prevents
 * descent while merely passing through the desired heading during a turn.
 *
 * @unit deg/s
 * @min 0.1
 * @max 30
 * @decimal 1
 * @increment 1
 * @group Precision Land
 */
PARAM_DEFINE_FLOAT(PLD_YAW_VEL, 5.0f);
