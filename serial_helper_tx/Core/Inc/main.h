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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
union u82int{
	int to_int;
	uint8_t buf[2];
};
union u82float{
	float to_float;
	uint8_t buf[4];
};

extern union u82int conv1;
extern union u82int conv2;
extern union u82int conv3;
extern union u82int conv4;

extern union u82float con1;
extern union u82float con2;
extern union u82float con3;
extern union u82float con4;

extern uint8_t rxbuff[18];

extern int count_rx;
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
#define PWM_NUM 10000
#define LORA_M0_Pin GPIO_PIN_13
#define LORA_M0_GPIO_Port GPIOC
#define LORA_M1_Pin GPIO_PIN_14
#define LORA_M1_GPIO_Port GPIOC
#define LORA_M2_Pin GPIO_PIN_15
#define LORA_M2_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_6
#define LED_R_GPIO_Port GPIOA
#define ICM42688_MISO_Pin GPIO_PIN_1
#define ICM42688_MISO_GPIO_Port GPIOB
#define ICM42688_SDA_Pin GPIO_PIN_2
#define ICM42688_SDA_GPIO_Port GPIOB
#define ICM42688_SCL_Pin GPIO_PIN_10
#define ICM42688_SCL_GPIO_Port GPIOB
#define ICM42688_CS_Pin GPIO_PIN_11
#define ICM42688_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define LED_R_SET 			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET)
#define LED_R_RESET 		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET)
#define LED_R_CHANGE 		HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin)

//#define FLYING_FISH 1
//#define CHANGE_STIFF_FISH 1
#define SERIAL_HELPER 1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
