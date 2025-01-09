#ifndef __CONTROL_H__
#define __CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "telecontrol.h"
#include "lora.h"
#include "pid.h"
#include <math.h>
#include "cpg.h"

#define SERVO_LOW 900
#define SERVO_HIGH 2100
#define SERVO_MEDIAN 1500
#define MOTOR_LOW 1000
#define MOTOR_HIGH 2000

void control_task(void);


#ifdef __cplusplus
}
#endif
#endif /*__CONTROL_H__ */