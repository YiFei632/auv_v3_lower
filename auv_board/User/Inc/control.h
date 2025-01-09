#ifndef __CONTROL_H__
#define __CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lora.h"
#include "telecontrol.h"
#include "tim.h"
#include "pid.h"
//#include "icm_42688.h"
#include "cpg.h"
#include "sim_SPItwo.h"

#define PWM_NUM 10000
#define SERVO_LOW 900
#define SERVO_HIGH 2100
#define SERVO_MEDIAN 1500




typedef enum
{
	servo_stop_mode,
	telecontrol_mode,
	imu_control_mode,
}servo_control_mode_t;

typedef enum
{
	stop_mode,
	torque_mode,
	velocity_mode,
	position_mode,
	cpg_mode
}motor_control_mode_t;

void control_task(void);

#ifdef __cplusplus
}
#endif
#endif /*__CONTROL_H__ */