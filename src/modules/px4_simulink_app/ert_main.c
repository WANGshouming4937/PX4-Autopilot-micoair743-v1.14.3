/*
 * File: ert_main.c
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

#include <stdio.h>
#include <stdlib.h>
#include "LW_ctrl_V1_11_260814.h"
#include "LW_ctrl_V1_11_260814_private.h"
#include "rtwtypes.h"
#include "limits.h"
#include "rt_nonfinite.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "fcntl.h"
#include "errno.h"
#include "debug.h"
#include "time.h"
#include "math.h"
#include "poll.h"
#include "px4_platform_common/posix.h"
#include "px4_platform_common/tasks.h"
#include "unistd.h"
#include "uORB/uORB.h"
#include "uORB/topics/sensor_combined.h"
#include "uORB/topics/vehicle_attitude.h"
#include "px4_platform_common/log.h"
#include "px4_platform_common/px4_config.h"
#include "px4_platform_common/tasks.h"
#include "px4_platform_common/posix.h"
#include "string.h"
#include "drivers/drv_pwm_output.h"
#include "uORB/topics/input_rc.h"
#include "drivers/drv_hrt.h"
#include "arch/board/board.h"
#include "sys/mount.h"
#include "sys/ioctl.h"
#include "sys/stat.h"
#include "perf/perf_counter.h"
#include "systemlib/err.h"
#include "parameters/param.h"
#include "nuttxinitialize.h"
#define UNUSED(x)                      x = x

/* Function prototype declaration*/
void exitFcn(int sig);
void *terminateTask(void *arg);
void *baseRateTask(void *arg);
void *subrateTask(void *arg);
volatile boolean_T runModel = true;
sem_t stopSem;
sem_t baserateTaskSem;
extern sem_t PX4_Simulink_Task_Sem;
extern struct hrt_call BaseRate_HRT;
extern void (*MW_model_init)(void);
extern void (*MW_model_term)(void);
void PX4_Simulink_Task_Callback(void *arg);
extern void PX4_Simulink_Task_Callback_Test(void *arg);
pthread_t schedulerThread;
pthread_t baseRateThread;
unsigned long threadJoinStatus[8];
int terminatingmodel = 0;
void *baseRateTask(void *arg)          //multi-rate single-tasking
{
  runModel = (rtmGetErrorStatus(LW_ctrl_V1_11_260814_M) == (NULL));
  while (runModel) {
    sem_wait(&baserateTaskSem);
    LW_ctrl_V1_11_260814_step();

    /* Get model outputs here */
    runModel = (rtmGetErrorStatus(LW_ctrl_V1_11_260814_M) == (NULL));
  }

  runModel = 0;
  terminateTask(arg);
  pthread_exit((void *)0);
  return NULL;
}

void exitFcn(int sig)
{
  UNUSED(sig);
  rtmSetErrorStatus(LW_ctrl_V1_11_260814_M, "stopping the model");
}

void *terminateTask(void *arg)
{
  UNUSED(arg);
  terminatingmodel = 1;
  printf("**terminating the model**\n");
  fflush(stdout);

  {
    runModel = 0;
  }

  sem_post(&stopSem);
  return NULL;
}

int px4_simulink_app_main(int argc, char *argv[]) //PX4 Simulink App
{
  rtmSetErrorStatus(LW_ctrl_V1_11_260814_M, 0);
  int PX4_Simulink_Task_PID = 0;
  if (argc < 1)
    px4_app_usage("missing command");
  if (!strcmp(argv[1], "start")) {
    if (g_baseRateLife == false) {
      /* Start the Simulink Tasks here */
      printf("**starting the model**\n");
      fflush(stdout);

      /* Initialize function pointers */
      MW_model_init = &LW_ctrl_V1_11_260814_initialize;
      MW_model_term = &LW_ctrl_V1_11_260814_terminate;

      /* Reset semaphore */
      g_baseRateLife = true;
      sem_init(&PX4_Simulink_Task_Sem,0,0);

      /* Create task */
      PX4_Simulink_Task_PID = task_create("PX4_Simulink_Task", 100, 2048, (void *)
        PX4_Simulink_Task_Callback, (void *) &PX4_Simulink_Task_PID);
    } else {
      PX4_INFO("\t**model is already running**\n");
      fflush(stdout);
    }

    exit(0);
  }

  if (!strcmp(argv[1], "stop")) {
    if (g_baseRateLife == true) {
      rtmSetErrorStatus(LW_ctrl_V1_11_260814_M, "Module finished");
      g_baseRateLife = false;
      sem_post(&baserateTaskSem);

      /* wait until the tasks completely finish */
      PX4_INFO("exiting model... waiting on stopSem...");

      /* Wait for stop semaphore */
      sem_wait(&stopSem);
      sem_post(&PX4_Simulink_Task_Sem);
    } else {
      PX4_INFO("\t**model is not running**\n");
      fflush(stdout);
    }

    exit(0);
  }

  if (!strcmp(argv[1], "status")) {
    if (g_baseRateLife) {
      PX4_INFO("\trunning\n");
    } else {
      PX4_INFO("\tnot started\n");
    }

    exit(0);
  }

  px4_app_usage("unrecognized command");
  return 1;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
