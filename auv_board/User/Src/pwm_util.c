/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       PWM的相关库文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#include "pwm_util.h"

PWM_PWMHandleTypeDef propeller_pwm[4];
PWM_PWMHandleTypeDef servo_pwm[4];

//Functions

void InitPWM(PWM_PWMHandleTypeDef* pwm,TIM_HandleTypeDef* htim,uint32_t ch){
	pwm->htim = htim;
	pwm->ch = ch;
	if (pwm->state == PWM_OFF) {
		HAL_TIM_PWM_Start(pwm->htim, pwm->ch); 
		pwm->state = PWM_ON;
	}
}

void StopPWM(PWM_PWMHandleTypeDef* pwm){
	if (pwm->state == PWM_ON) {
		HAL_TIM_PWM_Stop(pwm->htim, pwm->ch);
		pwm->state = PWM_OFF;
	}
}

//float pwm_temp;
void PWM_SetPWMDuty(PWM_PWMHandleTypeDef* pwm) {
	//pwm_temp=(PWM_PERIOD + 1)*(pwm->duty_current);
	__HAL_TIM_SET_COMPARE(pwm->htim,pwm->ch,PWM_NUM*(pwm->duty_current));
}

