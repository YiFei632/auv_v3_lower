#include "control.h"

float telecontrol_servo_value[4];
void telecontrol_mode_servo()
{
	if(telecontrol.is_receive)
	{
		float set_value[4];
		for(int i = 0; i < 4; i++)
		{
			set_value[i] = 1.0 * telecontrol.rocker[i] / (E70_433MT14S.type == short_data ? short_rocker_max : long_rocker_max) * (SERVO_HIGH - SERVO_LOW) + SERVO_LOW;
			if(telecontrol_servo_value[i] == 0)
				telecontrol_servo_value[i] = set_value[i];
			else
				telecontrol_servo_value[i] = 0.1 * set_value[i] + 0.9 * telecontrol_servo_value[i];//0.02*(set_value[i] - telecontrol_servo_value[i]);
		}
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint16_t)telecontrol_servo_value[0]);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, (uint16_t)telecontrol_servo_value[1]);
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, (uint16_t)telecontrol_servo_value[2]);
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (uint16_t)telecontrol_servo_value[3]);
		
		telecontrol_servo_value[3] = SERVO_MEDIAN;
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (uint16_t)telecontrol_servo_value[3]);
	}
}

float temp_data[4];
uint8_t timestamp;
float hz, A, derta;
float angle;
void telecontrol_mode_servo_sin()
{
	if(telecontrol.is_receive)
	{
		float tele_value[4];
		
		for(int i = 0; i < 4; i++)
		{
			tele_value[i] = 1.0 * telecontrol.rocker[i] / (E70_433MT14S.type == short_data ? short_rocker_max : long_rocker_max);
			if(temp_data[i] == 0)
				temp_data[i] = tele_value[i];
			else
				temp_data[i] = 0.1 * tele_value[i] + 0.9 * temp_data[i];//0.02*(set_value[i] - telecontrol_servo_value[i]);
		}
		
		
		if(timestamp == 100)
		{
			timestamp = 0;
			if(temp_data[2] > 0.6)
				hz = 20 * (temp_data[2] - 0.6) + 8;
			else
				hz = 0;
		}
		A = temp_data[1] * (SERVO_HIGH - SERVO_MEDIAN) * 3;
		derta = -(temp_data[0] - 0.5) * 3;
		timestamp++;
		hopf_cpg.offset = derta;
		hopf_cpg.omega = hz;
		if(hz != 0)
			angle = A * hopf_cpg_cal();
		
		for(int i = 0; i < 4; i++)
		{
			telecontrol_servo_value[i] = SERVO_MEDIAN + A * sin((timestamp / 100.) * hz * 6.28) + derta;
			telecontrol_servo_value[i] = angle + SERVO_MEDIAN;
		}
		
#ifdef FLYING_FISH
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint16_t)telecontrol_servo_value[0]);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, (uint16_t)telecontrol_servo_value[1]);
#endif
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, (uint16_t)telecontrol_servo_value[2]);
#ifdef CHANGE_STIFF_FISH
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (uint16_t)telecontrol_servo_value[3]);
#endif
	}
}

float telecontrol_motor_value[4];
void telecontrol_mode_motor()
{
	if(telecontrol.is_receive)
	{
		float set_value[4];
		for(int i = 0; i < 4; i++)
		{
			float rocker_temp = 1.0 * telecontrol.rocker[i] / (E70_433MT14S.type == short_data ? short_rocker_max : long_rocker_max);
			if (rocker_temp < 0.57)
				set_value[i] = MOTOR_LOW;
			else
				set_value[i] = 2 * (rocker_temp - 0.5) * (MOTOR_HIGH - MOTOR_LOW) + MOTOR_LOW;
			if(telecontrol_motor_value[i] == 0)
				telecontrol_motor_value[i] = set_value[i];
			else
				telecontrol_motor_value[i] = 0.1 * set_value[i] + 0.9 * telecontrol_motor_value[i];//0.02*(set_value[i] - telecontrol_servo_value[i]);
		}
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint16_t)telecontrol_servo_value[0]);
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, (uint16_t)telecontrol_motor_value[1]);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, (uint16_t)telecontrol_motor_value[2]);
		//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, (uint16_t)telecontrol_servo_value[3]);
	}
}

void telecontrol_mode_pump()
{
	uint16_t temp_pump[5];
	if(telecontrol.rocker[4] > 0.5 * (E70_433MT14S.type == short_data ? short_rocker_max : long_rocker_max) || telecontrol.is_receive == 0)
		temp_pump[4] = 0;
	else 
		temp_pump[4] = PWM_NUM-2;
	if(telecontrol.dial[0])
		temp_pump[0] = 0;
	else
		temp_pump[0] = PWM_NUM-2;
	if(telecontrol.dial[1])
		temp_pump[1] = 0;
	else
		temp_pump[1] = PWM_NUM-2;
	if(telecontrol.dial[2])
		temp_pump[2] = 0;
	else
		temp_pump[2] = 718;
	if(telecontrol.dial[3])
		temp_pump[3] = 0;
	else
		temp_pump[3] = 718;
	
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, temp_pump[4]);//pump
	
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, temp_pump[0]);//chage stiff 
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, temp_pump[1]);
#ifdef CHANGE_STIFF_FISH
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, temp_pump[2]);//serial chage stiff 
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, temp_pump[3]);
#endif
}

void set_motor()
{
	telecontrol_mode_motor();
}

void set_servo()
{
	//telecontrol_mode_servo();
	telecontrol_mode_servo_sin();
}

void set_pump()
{
	//telecontrol_mode_servo();
	telecontrol_mode_pump();
}

void control_task()
{	
#ifdef FLYING_FISH
	set_motor();
#endif
	set_servo();
#ifdef CHANGE_STIFF_FISH
	set_pump();
#endif
}