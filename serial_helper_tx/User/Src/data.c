#include "data.h"

uint8_t ADD8(uint8_t* str, int k)
{
	uint8_t temp = 0;
	for(int i = 0; i < k; i++)
		temp += str[i];
	return temp;
}

void get_telecontrol_data_short(uint8_t * buffer)
{
	telecontrol.key_state.key_left_rocker = (buffer[1] >> 5) & 0x01;
	telecontrol.key_state.key_right_rocker = (buffer[1] >> 4) & 0x01;
	for(int i=0;i<4;i++)
		telecontrol.dial[i] = (buffer[1] >> (3-i)) & 0x01;
	telecontrol.key_state.up = (buffer[2] >> 7) & 0x01;
	telecontrol.key_state.down = (buffer[2] >> 6) & 0x01;
	telecontrol.key_state.left = (buffer[2] >> 5) & 0x01;
	telecontrol.key_state.right = (buffer[2] >> 4) & 0x01;
	telecontrol.key_state.X = (buffer[2] >> 3) & 0x01;
	telecontrol.key_state.Y = (buffer[2] >> 2) & 0x01;
	telecontrol.key_state.A = (buffer[2] >> 1) & 0x01;
	telecontrol.key_state.B = (buffer[2] >> 0) & 0x01;
	for(int i=0;i<6;i++)
		telecontrol.rocker[i] = buffer[3+i];
}

void lora_data_full(uint8_t * data)
{
	telecontrol.is_receive = 0x01;
	if(data[0] == 0x55 && data[(E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1] == ADD8(data, (E70_433MT14S.type == short_data ? lora_LENGTH_short - 1 : lora_LENGTH - 1)))
		get_telecontrol_data_short(data);
}

uint8_t temp_lora, cmd[100];
void lora_data_test(uint8_t data)
{
	if(temp_lora == 0)
	{
		cmd[0] = data;
		temp_lora++;
	}
	else if(temp_lora == 1)
	{
		cmd[1] = data;
		temp_lora++;
	}
	else if(temp_lora == 2)
	{
		cmd[2] = data;
		temp_lora++;
	}
	else if(temp_lora == 3)
	{
		cmd[3] = data;
		temp_lora++;
	}
	else if(temp_lora == 4)
	{
		cmd[4] = data;
		temp_lora++;
	}
	else if(temp_lora == 5)
	{
		cmd[5] = data;
		temp_lora++;
	}
	else
	{
		cmd[temp_lora] = data;
		temp_lora++;
	}
}

void lora_data(uint8_t data)
{
	if(temp_lora == 0 && data == 0x55)
	{
		RxBuffer_2[temp_lora] = data;
		temp_lora++;
		return;
	}
	else if(temp_lora == 0 && data != 0x55)
	{
		memset((void *)RxBuffer_2,0,RX_BUFFER_SIZE);
		temp_lora=0;
		return;
	}
	
	if(temp_lora >= 1 && temp_lora < ((E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1))
	{
		RxBuffer_2[temp_lora] = data;
		temp_lora++;
		return;
	}
	
	if(E70_433MT14S.type == long_full_data)
	{
		if(temp_lora == lora_LENGTH + 1 && data == 0xFF)
		{
			RxBuffer_2[temp_lora] = data;
			temp_lora++;
			return;
		}
		else if(temp_lora == lora_LENGTH + 1 && data != 0xFF)
		{
			memset((void *)RxBuffer_2,0,RX_BUFFER_SIZE);
			temp_lora=0;
			return;
		}
	}
	
	if(temp_lora == ((E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1) && \
		data == ADD8(RxBuffer_2, (E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1))
	{
		if (E70_433MT14S.type == short_data)
			get_telecontrol_data_short(RxBuffer_2);
		telecontrol.is_receive = 1;
		//LED_G_CHANGE;
		temp_lora=0;
		memset((void *)RxBuffer_2,0,RX_BUFFER_SIZE);
		return;
	}
	else
	{
		temp_lora = 0;
		memset((void *)RxBuffer_2,0,RX_BUFFER_SIZE);
	}
}

void receive_conv(uint8_t data){
	if(temp_lora == 0 && data == 0x77){
		rxbuff[temp_lora] = data;
		HAL_UART_Transmit(&huart1, &data, 1, 0xFF);
		temp_lora++;
		return;
	}
	else if(temp_lora == 0 && data != 0x77){
		//rxbuff[temp_lora] = data;
		temp_lora = 0;
		return;
	}
	else if(temp_lora > 0 && data != 0xFF){
		rxbuff[temp_lora] = data;
		HAL_UART_Transmit(&huart1, &data, 1, 0xFF);
		temp_lora++;
		return;
	}
	else if(temp_lora > 0 && data == 0xFF){
		rxbuff[temp_lora] = data;
		HAL_UART_Transmit(&huart1, &data, 1, 0xFF);
		temp_lora = 0;
//		for(int i = 0;i < 18;i++){
//			HAL_UART_Transmit(&huart1, &rxbuff[i], 1, 0xFF);
//		}
		return;
	}
}