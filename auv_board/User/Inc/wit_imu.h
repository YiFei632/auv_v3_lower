#ifndef __WIT_IMU_H__
#define __WIT_IMU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "lora.h"

#define WIT_ADDR 0x50
// �����������ٶ�gֵ
#define GRAVITY 9.8f

// ��������������ֽ���ϳ�16λ����
#define COMBINE_16BIT(L, H) (((int16_t)(H) << 8) | (L))

typedef struct{
	uint8_t addr;
	float gyro[3];
	float angle[3];
	
	
}wit_imu_t;

extern wit_imu_t IMU2;

void wit_imu_init(wit_imu_t* imu);
//void get_wit_imu_data(wit_imu_t* imu);
void get_wit_imu_angle(wit_imu_t* imu);
void calc_wit_angle(wit_imu_t* imu, uint8_t rxbuff[]);

#ifdef __cplusplus
}
#endif
#endif /*__KERNAL_H__ */