#include "wit_imu.h"

wit_imu_t IMU2;

uint8_t rxbuff[11];
uint8_t txbuff[8];

void wit_imu_init(wit_imu_t* imu){
	imu->addr = WIT_ADDR;
	for(int i = 0;i < 3;i++){
		imu->gyro[i] = 0.0f;
		imu->angle[i] = 0.0f;
	}
	
}

void get_wit_imu_angle(wit_imu_t* imu){
	txbuff[0] = imu->addr;
	txbuff[1] = 0x03;
	txbuff[2] = 0x00;
	txbuff[3] = 0x3D;
	txbuff[4] = 0x00;
	txbuff[5] = 0x03;
	txbuff[6] = 0x99;
	txbuff[7] = 0x86;
	
	TXEN_485;
	for(int i = 0;i < 8;i++){
		HAL_UART_Transmit(&huart1,&txbuff[i],1,1000);
	}
	RXEN_485;
}

void calc_wit_angle(wit_imu_t* imu, uint8_t rxbuff[]){
	//uint32_t reg;
	//do{
		//reg = HAL_UART_Receive(&huart1,rxbuff,sizeof(rxbuff),0x02);
	//}while(reg != HAL_OK);
	
	int16_t Roll = COMBINE_16BIT(rxbuff[4], rxbuff[3]);
	int16_t Pitch = COMBINE_16BIT(rxbuff[6], rxbuff[5]);
	int16_t Yaw = COMBINE_16BIT(rxbuff[8], rxbuff[7]);
	
	imu->angle[0] = Roll / 32768.0f * 180.0f;  // 滚转角 (单位: °)
	imu->angle[1] = Pitch / 32768.0f * 180.0f; // 俯仰角 (单位: °)
	imu->angle[2] = Yaw / 32768.0f * 180.0f;   // 偏航角 (单位: °)
}
//	txbuff[0] = imu->addr;
//	txbuff[1] = 0x03;
//	txbuff[2] = 0x00;
//	txbuff[3] = 0x3D;
//	txbuff[4] = 0x00;
//	txbuff[5] = 0x03;
//	txbuff[6] = 0x99;
//	txbuff[7] = 0x86;
//	
//	TXEN_485;
//	HAL_UART_Transmit(&huart1,txbuff,sizeof(txbuff),0x02);
//	RXEN_485;
//	//uint32_t reg;
//	//do{
//		reg = HAL_UART_Receive(&huart1,rxbuff,sizeof(rxbuff),0x02);
//	//}while(reg != HAL_OK);
//	
//	int16_t Roll = COMBINE_16BIT(rxbuff[4], rxbuff[3]);
//	int16_t Pitch = COMBINE_16BIT(rxbuff[6], rxbuff[5]);
//	int16_t Yaw = COMBINE_16BIT(rxbuff[8], rxbuff[7]);
//	
//	imu->angle[0] = Roll / 32768.0f * 180.0f;  // 滚转角 (单位: °)
//	imu->angle[1] = Pitch / 32768.0f * 180.0f; // 俯仰角 (单位: °)
//	imu->angle[2] = Yaw / 32768.0f * 180.0f;   // 偏航角 (单位: °)
//	
//}