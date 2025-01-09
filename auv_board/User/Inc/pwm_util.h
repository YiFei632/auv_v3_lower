/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
 #ifndef __PWM_UTIL_H
#define __PWM_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "main.h"
#include "tim.h"
	
#define LEFT      0
#define RIGHT     1
#define FRONT     2
#define BACK      3

typedef enum {
	PWM_OFF = 0,
	PWM_ON  = 1

} PWM_PWMStateEnum;

typedef struct {
	TIM_HandleTypeDef* htim;	
	uint32_t ch;				
	float duty_current;
	PWM_PWMStateEnum state;		
	
} PWM_PWMHandleTypeDef;//PWM Handler

extern PWM_PWMHandleTypeDef propeller_pwm[4];
extern PWM_PWMHandleTypeDef servo_pwm[4];

void InitPWM(PWM_PWMHandleTypeDef* pwm,TIM_HandleTypeDef* htim,uint32_t ch);
void StopPWM(PWM_PWMHandleTypeDef* pwm);
void PWM_SetPWMDuty(PWM_PWMHandleTypeDef* pwm);


#endif
#ifdef __cplusplus
}
#endif