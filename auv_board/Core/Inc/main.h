/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
union u82int{
	int to_int;
	uint8_t buf[2];
};
union float2u8{
	float to_float;
	uint8_t buf[4];
};
//extern union u82int conv1;
//extern union u82int conv2;
//extern union u82int conv3;
//extern union u82int conv4;
extern union float2u8 conv1;
extern union float2u8 conv2;
extern union float2u8 conv3;
extern union float2u8 conv4;


extern uint8_t txbuff_rf200[16];
extern uint8_t rxbuff_e70[18];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_NUM 20000
#define serial1_TX_Pin GPIO_PIN_14
#define serial1_TX_GPIO_Port GPIOC
#define serial1_RX_Pin GPIO_PIN_13
#define serial1_RX_GPIO_Port GPIOC
#define RS485_EN_Pin GPIO_PIN_9
#define RS485_EN_GPIO_Port GPIOB
#define ICM42688_CS_Pin GPIO_PIN_1
#define ICM42688_CS_GPIO_Port GPIOE
#define ICM42688_SDA_Pin GPIO_PIN_7
#define ICM42688_SDA_GPIO_Port GPIOD
#define WF5803_SDA_Pin GPIO_PIN_7
#define WF5803_SDA_GPIO_Port GPIOC
#define WF5803_SCL_Pin GPIO_PIN_6
#define WF5803_SCL_GPIO_Port GPIOC
#define ICM42688_SCL_Pin GPIO_PIN_5
#define ICM42688_SCL_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOE
#define ICM42688_MISO_Pin GPIO_PIN_6
#define ICM42688_MISO_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_7
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_13
#define LED_G_GPIO_Port GPIOE
#define LORA_M0_Pin GPIO_PIN_12
#define LORA_M0_GPIO_Port GPIOB
#define LORA_M1_Pin GPIO_PIN_8
#define LORA_M1_GPIO_Port GPIOD
#define LORA_M2_Pin GPIO_PIN_12
#define LORA_M2_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */
#define LED_G_SET 			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET)
#define LED_G_RESET 		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET)
#define LED_G_CHANGE 		HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin)
#define LED_R_SET 			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET)
#define LED_R_RESET 		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET)
#define LED_R_CHANGE 		HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin)
#define LED_B_SET 			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET)
#define LED_B_RESET 		HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET)
#define LED_B_CHANGE 		HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin)

#define TXEN_485   HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);        		//485高电平发数据
#define RXEN_485   HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);         //485低电平收数据，默认状态

#define FLYING_FISH
//#define CHANGE_STIFF_FISH
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
