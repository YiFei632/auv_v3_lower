#include "kernal.h"

void kernal()
{
	Get_ICM42688_Data();
	control_task();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//��ʱ���жϺ���
{
	if(htim == &htim3)//��ʱ���ж�11   1hz
	{
		LED_R_CHANGE;
	}
	if(htim == &htim2)//��ʱ���ж�10  100hz �ж����ȼ���
	{
		kernal();
	}
}

void stop_all_IT()
{
	HAL_TIM_Base_Stop_IT(&htim2);
	HAL_TIM_Base_Stop_IT(&htim3);
	
	__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
}