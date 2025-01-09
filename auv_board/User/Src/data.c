#include "data.h"

uint8_t ADD8(uint8_t* str, int k)
{
	uint8_t temp = 0;
	for(int i = 0; i < k; i++)
		temp += str[i];
	return temp;
}

void get_telecontrol_data_short(uint8_t * buffer)//Rewrite this function to receive command data;
{
	if((buffer[1] >> 6) & 0x01)
	{
//		switch(buffer[2])
//		{
//		case 0x00:{
//			sd_cmd = SD_READ_INFO;
//			break;
//		}
//		case 0x01:{
//			sd_cmd = SD_INFO_PLUS;
//			break;
//		}
//		case 0x02:{
//			sd_cmd = SD_INFO_MINUS;
//			break;
//		}
//		case 0x03:{
//			sd_cmd = SD_READ_DATA;
//			break;
//		}
//		default:{
//			sd_cmd = SD_NO_COMMAND;
//			break;
//		}
//		}
		if(buffer[2] == 0x01){
			sd_cmd = SD_READ_INFO;
		}else if(buffer[2] == 0x02){
			sd_cmd = SD_INFO_PLUS;
		}else if(buffer[2] == 0x03){
			sd_cmd = SD_INFO_MINUS;
		}else if(buffer[2] == 0x04){
			sd_cmd = SD_READ_DATA;
		}else if(buffer[2] == 0x05){
			sd_cmd = RECORD_DATA;
		}else if(buffer[2] == 0x06){
			sd_cmd = SENT_DATA;
		}else{
			sd_cmd = SD_NO_COMMAND;
		}
	}
	else
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
		for(int i=0;i<6;i++){
			telecontrol.rocker[i] = buffer[3+i];
		}
	}
}

void lora_data_full(uint8_t * data)
{
	telecontrol.is_receive = 0x01;
	if(data[0] == 0x55 && data[(E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1] == ADD8(data, (E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1))
		get_telecontrol_data_short(data);
	else if(data[1] == 0x55 && data[0] == ADD8(&data[1], (E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1))
	{
		uint8_t temp = data[0];
		for(int k = 0; k < ((E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1); k++)
			data[k] = data[k + 1];
		data[(E70_433MT14S.type == short_data ? lora_LENGTH_short : lora_LENGTH) - 1] = temp;
		get_telecontrol_data_short(data);
	}
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

void RS485_data(uint8_t data)
{
	
}

uint8_t motor_ctl_data[12];
void motor_control(uint8_t motor_num, uint8_t mode, int16_t data)
{
	uint8_t _cnt=0;
	int16_t temp;
	
	motor_ctl_data[_cnt++] = 0xAA; //帧头
	motor_ctl_data[_cnt++] = 0xAF; 
	motor_ctl_data[_cnt++] = motor_num; 
	motor_ctl_data[_cnt++] = mode; 
	motor_ctl_data[_cnt++] = 2; //数据个数

	temp = data;
	motor_ctl_data[_cnt++] = (temp >> 8) & 0xFF; 
	motor_ctl_data[_cnt++] = (temp >> 0) & 0xFF;

	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += motor_ctl_data[i];  //校验和
		
	motor_ctl_data[_cnt++]=sum;
	
	TXEN_485;
	for(uint8_t i=0;i<_cnt;i++)
	{
		HAL_UART_Transmit(&huart1, &motor_ctl_data[i], 1, 1000);
		while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);
	}
	RXEN_485;
}

void motor_control_cpg(uint8_t motor_num)
{
	uint8_t _cnt=0;
	
	motor_ctl_data[_cnt++] = 0xAA; //帧头
	motor_ctl_data[_cnt++] = 0xAF; 
	motor_ctl_data[_cnt++] = motor_num; 
	motor_ctl_data[_cnt++] = cpg_mode; 
	motor_ctl_data[_cnt++] = 4; //数据个数
	
	for(int k = 0; k < 4; k ++)
		motor_ctl_data[_cnt++] = telecontrol.rocker[k];
	
	uint8_t sum = 0;
	for(uint8_t i=0;i<_cnt;i++)
		sum += motor_ctl_data[i];  //校验和
		
	motor_ctl_data[_cnt++]=sum;
	
	TXEN_485;
	for(uint8_t i=0;i<_cnt;i++)
	{
		HAL_UART_Transmit(&huart1, &motor_ctl_data[i], 1, 1000);
		while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX);
	}
	RXEN_485;
}

void receive_conv(uint8_t data){
	if(temp_lora == 0 && data == 0x77){
		rxbuff_e70[temp_lora] = data;
		temp_lora++;
		return;
	}
	else if(temp_lora == 0 && data != 0x77){
//		rxbuff_e70[temp_lora] = data;
//		temp_lora++;
		temp_lora = 0;
		return;
	}
	else if(temp_lora > 0 && data != 0xFF){
		rxbuff_e70[temp_lora] = data;
		temp_lora++;
		return;
	}
	else if(temp_lora > 0 && data == 0xFF){
		rxbuff_e70[temp_lora] = data;
		temp_lora = 0;
//		conv1.buf[0] = rxbuff[1];
//		conv1.buf[1] = rxbuff[2];
//		conv2.buf[0] = rxbuff[3];
//		conv2.buf[1] = rxbuff[4];
//		conv3.buf[0] = rxbuff[5];
//		conv3.buf[1] = rxbuff[6];
//		conv4.buf[0] = rxbuff[7];
//		conv4.buf[1] = rxbuff[8];
		conv1.buf[0] = rxbuff_e70[1];
		conv1.buf[1] = rxbuff_e70[2];
		conv1.buf[2] = rxbuff_e70[3];
		conv1.buf[3] = rxbuff_e70[4];
		conv2.buf[0] = rxbuff_e70[5];
		conv2.buf[1] = rxbuff_e70[6];
		conv2.buf[2] = rxbuff_e70[7];
		conv2.buf[3] = rxbuff_e70[8];
		conv3.buf[0] = rxbuff_e70[9];
		conv3.buf[1] = rxbuff_e70[10];
		conv3.buf[2] = rxbuff_e70[11];
		conv3.buf[3] = rxbuff_e70[12];
		conv4.buf[0] = rxbuff_e70[13];
		conv4.buf[1] = rxbuff_e70[14];
		conv4.buf[2] = rxbuff_e70[15];
		conv4.buf[3] = rxbuff_e70[16];
		return;
	}
}