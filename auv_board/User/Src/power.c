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

void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)//ADCת������жϻص�������ÿ��ADCת����ɴ����жϵ��ñ�����
{
	if(hadc == &hadc1)
	{
		//��ȡת�����	
		//rocker_value = HAL_ADC_GetValue(hadc);		//ʹ�ô˺������Ի�ȡADC������ֵ
		
		HAL_ADC_Stop_DMA(&hadc1);//�ر�DMA��ADC�ɼ�
	}
	else if(hadc == &hadc2)
	{
		//��ȡת�����	
		//rocker_value = HAL_ADC_GetValue(hadc);		//ʹ�ô˺������Ի�ȡADC������ֵ
		
		HAL_ADC_Stop_DMA(&hadc2);//�ر�DMA��ADC�ɼ�
	}
}