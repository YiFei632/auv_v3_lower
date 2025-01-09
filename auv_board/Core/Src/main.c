/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sim_SPItwo.h"
#include "wf5803.h"
//#include "ms5837_dev.h"
#include "pwm_util.h"
#include "motor_dev.h"
#include "lora.h"
#include "SDCard.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
union float2u8 conv1;//Left Thrust
union float2u8 conv2;//Right Thrust
union float2u8 conv3;//Front and Back Thrust
union float2u8 conv4;//Front and Back Servo

uint8_t txbuff_rf200[16];
uint8_t rxbuff_e70[18];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM15_Init();
  MX_USART2_UART_Init();
  MX_TIM16_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_TIM12_Init();
  /* USER CODE BEGIN 2 */
	icm42688_init();
	wf5803_init();
	//ms5837_init(&sensor,MS5837_30BA,freshwater);
	
	E70_433MT14S_init();
	SDCardInit();
	
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	
	
	/*    Initialization    */
	//uint8_t ret = ms5837_init(&sensor,MS5837_30BA,freshwater);
	
	InitPWM(&propeller_pwm[0],&htim2,TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,1500);
	InitPWM(&propeller_pwm[1],&htim2,TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,1500);
	InitPWM(&propeller_pwm[2],&htim2,TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,1500);
	InitPWM(&propeller_pwm[3],&htim2,TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,1500);
	HAL_Delay(5000);
	
	InitPWM(&servo_pwm[0],&htim3,TIM_CHANNEL_1);
	InitPWM(&servo_pwm[1],&htim3,TIM_CHANNEL_2);
	InitPWM(&servo_pwm[2],&htim3,TIM_CHANNEL_3);
	InitPWM(&servo_pwm[3],&htim3,TIM_CHANNEL_4);
	//²ÎÊýÆô¶¯
	propeller_init(&auv_propeller[0],PROPELLER_LEFT);
	propeller_init(&auv_propeller[1],PROPELLER_RIGHT);
	propeller_init(&auv_propeller[2],PROPELLER_FORWARD);
	propeller_init(&auv_propeller[3],PROPELLER_BACK);
	
	servo_init(&auv_servo[0],SERVO_LEFT);
	servo_init(&auv_servo[1],SERVO_RIGHT);
	servo_init(&auv_servo[2],SERVO_FORWARD);
	servo_init(&auv_servo[3],SERVO_BACK);
	is_record = 0;
	is_record_flash=0;
	(&auv_remote)->status = NO_MOOD;
	
	HAL_Delay(100);
	
	HAL_TIM_Base_Start_IT(&htim15);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim12);
	//Serial test!!!
//	conv1.to_int = 300;
//	conv2.to_int = 600;
//	conv3.to_int = 900;
//	conv4.to_int = 1200;
//	
//	txbuff[0] = 0x55;
//	txbuff[1] = conv1.buf[0];
//	txbuff[2] = conv1.buf[1];
//	txbuff[3] = conv2.buf[0];
//	txbuff[4] = conv2.buf[1];
//	txbuff[5] = conv3.buf[0];
//	txbuff[6] = conv3.buf[1];
//	txbuff[7] = conv4.buf[0];
//	txbuff[8] = conv4.buf[1];
//	txbuff[9] = 0x77;
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/*Deleted for a submerge reason*/
//		if(telecontrol.dial[3] == 1){
//			//is_record = 1;
//		LED_G_CHANGE;
//			if(has_storaged == 0){
//				is_record = 1;
//			}
//		}
//		if(is_storage)
//		{
//			SDCardWrite();
//			is_storage = 0;
//			is_record = 0;
//			has_storaged = 1;
		//HAL_GPIO_WritePin(serial1_TX_GPIO_Port, serial1_TX_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(serial1_TX_GPIO_Port, serial1_TX_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		
//		SDCardCmd();
		
		/*write for a submerge reason*/
		if(telecontrol.dial[0] == 0 && telecontrol.dial[1] == 0){
			(&auv_remote)->status = NO_MOOD;
		}
		
		if(telecontrol.rocker[4] >= 0x00F0){
			if(is_record_flash == 0){
				sd_cmd = RECORD_DATA;
			}
			
		}
//		}else if(telecontrol.dial[3] == 1){
//			sd_cmd = SENT_DATA;
//		}

		
		/*a single test about reading sd card data*/
		
//		if(telecontrol.dial[2] == 1){
//			sd_cmd = SD_READ_DATA;
//		}else{
//			sd_cmd = SD_NO_COMMAND;
//		}
		
		//sd_cmd = SD_NO_COMMAND;
		
		HAL_Delay(2);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f){
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 1000);    
    return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
