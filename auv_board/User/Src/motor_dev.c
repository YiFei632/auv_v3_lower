/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       螺旋桨和舵机的相关文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
#include "motor_dev.h"

propeller_HandleTypedef auv_propeller[4];
servo_HandleTypedef auv_servo[4];

void propeller_init(propeller_HandleTypedef* propeller, propeller_id_enum id){
	propeller->id = id;
	propeller->speed_ref = 0.0;
	propeller->speed_fdb = 0.0;
	propeller->speed_calc = PROPELLER_MIDDLE;
	propeller->speed_calc_pwm = PROPELLER_MIDDLE_PWM;
	
//	(&propeller_pwm[(int)(propeller->id)])->duty_current = propeller->speed_calc_pwm;
//	PWM_SetPWMDuty(&propeller_pwm[(int)(propeller->id)]);
//	HAL_Delay(100);
}

void servo_init(servo_HandleTypedef* servo, servo_id_enum id){
	servo->id = id;
	servo->angle_ref = 0.0;
	servo->angle_fdb = 0.0;
	servo->angle_calc = SERVO_MIDDLE;
	servo->angle_calc_pwm = SERVO_MIDDLE_PWM;
	
	(&servo_pwm[(int)(servo->id)])->duty_current = servo->angle_calc_pwm;
	PWM_SetPWMDuty(&servo_pwm[(int)(servo->id)]);
	HAL_Delay(10);
}



void propeller_calcspeed(propeller_HandleTypedef* propeller){
	if((propeller->speed_calc) >= 0.0){
		propeller->speed_calc_pwm = data_calc(propeller->speed_calc,PROPELLER_MAX,PROPELLER_MIDDLE,PROPELLER_MAX_PWM,PROPELLER_MIDDLE_PWM);
	}else{
		propeller->speed_calc_pwm = data_calc(propeller->speed_calc,PROPELLER_MIDDLE,PROPELLER_MIN,PROPELLER_MIDDLE_PWM,PROPELLER_MIN_PWM);
	}
	
//	if(fabs(propeller->speed_calc_pwm - PROPELLER_MIDDLE_PWM) <= 0.01){
//		propeller->speed_calc_pwm = PROPELLER_MIDDLE_PWM;
//	}
	
	(&propeller_pwm[(int)(propeller->id)])->duty_current = PROPELLER_PWM_ERR - (propeller->speed_calc_pwm);
	PWM_SetPWMDuty(&propeller_pwm[(int)(propeller->id)]);
	//HAL_Delay(10);
}

void servo_calcangle(servo_HandleTypedef* servo){
	servo->angle_calc_pwm = data_calc(servo->angle_calc,SERVO_MAX,SERVO_MIN,SERVO_MAX_PWM,SERVO_MIN_PWM);
	
	(&servo_pwm[(int)(servo->id)])->duty_current = servo->angle_calc_pwm;
	PWM_SetPWMDuty(&servo_pwm[(int)(servo->id)]);
	//HAL_Delay(10);
}
