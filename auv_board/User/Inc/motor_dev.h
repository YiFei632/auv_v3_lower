/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#ifndef __MOTOR_DEV_H
#define __MOTOR_DEV_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "pwm_util.h"
#include "mathtool_lib.h"
	
#define PI 3.1415926f
	
#define PROPELLER_MIDDLE     0.0f
#define PROPELLER_MIN        -18.00f
#define PROPELLER_MAX        18.00f

#define PROPELLER_MIDDLE_PWM 0.075f
#define PROPELLER_MIN_PWM    0.06f
#define PROPELLER_MAX_PWM    0.09f

#define PROPELLER_PWM_ERR    0.15f //find some errors, set this to solve.
	

#define SERVO_MIN            -0.75*PI
#define SERVO_MIDDLE         0.0
#define SERVO_MAX            0.75*PI
	
#define SERVO_MIN_PWM        0.025f
#define SERVO_MIDDLE_PWM     0.075f
#define SERVO_MAX_PWM        0.125f
	
            
	
typedef enum{
	PROPELLER_LEFT = 0x00,
	PROPELLER_RIGHT = 0x01,
	PROPELLER_FORWARD = 0x02,
	PROPELLER_BACK = 0x03
}propeller_id_enum;

typedef enum{
	SERVO_LEFT = 0x00,
	SERVO_RIGHT = 0x01,
	SERVO_FORWARD = 0x02,
	SERVO_BACK = 0x03
}servo_id_enum;

typedef struct{
	propeller_id_enum id;
	float speed_ref;
	float speed_fdb;
	float speed_calc;
	float speed_calc_pwm;
}propeller_HandleTypedef;

typedef struct{
	servo_id_enum id;
	float angle_ref;
	float angle_fdb;
	float angle_calc;
	float angle_calc_pwm;
}servo_HandleTypedef;

extern propeller_HandleTypedef auv_propeller[4];
extern servo_HandleTypedef auv_servo[4];

void propeller_init(propeller_HandleTypedef* propeller, propeller_id_enum id);
void servo_init(servo_HandleTypedef* servo, servo_id_enum id);

void propeller_calcspeed(propeller_HandleTypedef* propeller);
void servo_calcangle(servo_HandleTypedef* servo);

#endif
#ifdef __cplusplus
}
#endif
