#include "power.h"

power_t power=
{
	.k = 3.3846e-04,
	.b = -2.6238
};

void get_power_data()
{
//	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)(&power.raw_data), 1);
	HAL_ADC_Start_DMA(&hadc2,(uint32_t *)(&power.raw_data), 2);
	power.voltage = power.k * power.raw_data[0] + power.b;
}

void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//ADC转换完成中断回调函数，每次ADC转换完成触发中断调用本函数
{
	if(hadc == &hadc1)
	{
		//获取转换结果	
		//rocker_value = HAL_ADC_GetValue(hadc);		//使用此函数可以获取ADC采样的值
		
		HAL_ADC_Stop_DMA(&hadc1);//关闭DMA的ADC采集
	}
	else if(hadc == &hadc2)
	{
		//获取转换结果	
		//rocker_value = HAL_ADC_GetValue(hadc);		//使用此函数可以获取ADC采样的值
		
		HAL_ADC_Stop_DMA(&hadc2);//关闭DMA的ADC采集
	}
}