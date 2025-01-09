#include "lora.h"

uint8_t RxBuffer_1[RX_BUFFER_SIZE], RxBuffer_2[RX_BUFFER_SIZE], RxBuffer_3[RX_BUFFER_SIZE];   //接收数据
//uint8_t RxBuffer_CMD[RX_BUFFER_SIZE];//Receive Command Data
uint8_t aRxBuffer_1, aRxBuffer_2, aRxBuffer_3;//HAL库使的串口接收缓冲
uint8_t ConfigStr[6] = {0xC0, 0x00, 0x00, 0x19, 0x04, 0x1C};
uint8_t TestStr[3] = {0xC1, 0xC1, 0xC1};

sdcard_command sd_cmd;//sdcard command

E70_433MT14S_t E70_433MT14S = 
{
	.type = short_data,
	.is_init = 0
};

void lora_mode(uint8_t M0, uint8_t M1, uint8_t M2)
{
	HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, (M0 == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET));
	HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, (M1 == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET));
	HAL_GPIO_WritePin(LORA_M2_GPIO_Port, LORA_M2_Pin, (M2 == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET));
}

void lora_mode_contact()
{
	lora_mode(0, 1, 0);
}

void lora_mode_contact_continuous()
{
	lora_mode(1, 0, 0);
}

void lora_mode_config()
{
	lora_mode(1, 1, 0);
}

void lora_set_baudrate()
{
	lora_mode_config();
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, ConfigStr, 6, 0xff);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, TestStr, 3, 0xFF);
	HAL_Delay(1000);
	//lora_mode_contact();
}

void E70_433MT14S_init()
{
	//lora_set_baudrate();
	sd_cmd = SD_NO_COMMAND;
	lora_mode_contact_continuous();
	HAL_UART_Receive_IT(&huart2, &aRxBuffer_2, 1);
	//HAL_UART_Receive_DMA(&huart2, RxBuffer_2, E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH);
	E70_433MT14S.is_init = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//串口1  连接RS485
	{
		RS485_data(aRxBuffer_1);
//		HAL_UART_Transmit(&huart3, &aRxBuffer_1, 1, 1000);
//		while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX);
		HAL_UART_Receive_IT(&huart1, &aRxBuffer_1, 1);
	}
//	if(huart->Instance==USART2)//串口2  预留备用
//	{
////		HAL_UART_Transmit(&huart3, &aRxBuffer_1, 1, 1000);
////		while(HAL_UART_GetState(&huart3) == HAL_UART_STATE_BUSY_TX);
//		HAL_UART_Receive_IT(&huart2, &aRxBuffer_2, 1);
//	}
	if(huart->Instance==USART2)//串口2  连接E70_433MT14S无线通信模块
	{
		receive_conv(aRxBuffer_2);
		HAL_UART_Receive_IT(&huart2, &aRxBuffer_2, 1);
		
	}
}