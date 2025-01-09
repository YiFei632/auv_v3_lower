#include "kernal.h"

void kernal()
{
	Get_ICM42688_Data();
	control_task();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器中断函数
{
	if(htim == &htim3)//定时器中断11   1hz
	{
		LED_R_CHANGE;
	}
	if(htim == &htim2)//定时器中断10  100hz 中断优先级高
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