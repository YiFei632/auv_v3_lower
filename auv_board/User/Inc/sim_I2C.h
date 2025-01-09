#ifndef __SIM_I2C_H__
#define __SIM_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define CPU_FREQUENCY_MHZ    480		// STM32Ê±ÖÓÖ÷Æµ

#define DeviceAddress_AS5600 			0X36 << 1
#define DeviceAddress_ICM42688 		0X69 << 1          //AD0_LOW 0X68      AD0_HIGH 0X69
#define DeviceAddress_WF5803	 		0XDA

typedef struct
{
	GPIO_TypeDef * GPIO_PORT;
	uint16_t GPIO_PIN;
	GPIO_PinState defalt_state;
}User_PIN_t;

typedef struct
{
	User_PIN_t SCL;
	User_PIN_t SDA;
	User_PIN_t CS;
	User_PIN_t MISO;
	uint8_t delay_time;
	uint8_t ack[3];
}sim_I2C_t;

void delay_us(__IO uint32_t delay);
uint8_t IIC_ReadOneByte(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr);
void IIC_WriteOneByte(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t RA, uint8_t data_byte);
void IIC_ReadBytes(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr, uint8_t num, uint8_t * data);
uint8_t IIC_Test(sim_I2C_t * i2c, uint8_t * addr);

#ifdef __cplusplus
}
#endif
#endif /*__SIM_I2C_H__ */