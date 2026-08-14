/****************************************************************************
 *
 *   Copyright (c) 2022 PX4 Development Team. All rights reserved.
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
 * Maximum radius of orbit
 *
 * @unit m
 * @min 1.0
 * @max 10000.0
 * @increment 0.5
 * @decimal 1
 * @group FlightTaskOrbit
 */
PARAM_DEFINE_FLOAT(MC_ORBIT_RAD_MAX, 1000.0f);

/**
 * Orbit velocity
 *
 * Target tangential velocity during orbit.
 * If set to a negative value, it will use the default cruise speed (MPC_XY_CRUISE).
 *
 * @unit m/s
 * @min 1.0
 * @max 20.0
 * @increment 0.5
 * @decimal 1
 * @group FlightTaskOrbit
 */
PARAM_DEFINE_FLOAT(MC_ORBIT_VEL, 5.0f);

/**
 * Orbit acceleration limit
 *
 * Centripetal  tangential acceleration during orbit.
 * If the combination of velocity and radius exceeds this acceleration, the velocity will be reduced to ensure the acceleration limit is not exceeded.
 * @unit m/s^2
 * @min 0.1
 * @max 10.0
 * @increment 0.1
 * @decimal 1
 * @group FlightTaskOrbit
 */
PARAM_DEFINE_FLOAT(MC_ORBIT_ACC_MAX, 3.0f);

/**
 * Yaw behaviour of orbit
 *
 * Defines how the vehicle heads during the orbit.
 *
 * @value 0 Point to Center (Face Center)
 * @value 1 Hold initial heading (Hold Initial)
 * @value 2 Uncontrolled (Uncontrolled)
 * @value 3 Tangent (Face Forward)
 * @value 4 Rc controlled (Rc Controlled)
 * @group FlightTaskOrbit
 */
PARAM_DEFINE_INT32(MC_ORBIT_YAW, 3);

