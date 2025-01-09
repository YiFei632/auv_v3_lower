/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       关于深度传感器ms5837的处理
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */

#include "ms5837_dev.h"

//sim_I2C_t i2c2 = 
//{
//	.SCL = 
//	{
//		.GPIO_PORT = WF5803_SCL_GPIO_Port,
//		.GPIO_PIN = WF5803_SCL_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
//	.SDA = 
//	{
//		.GPIO_PORT = WF5803_SDA_GPIO_Port,
//		.GPIO_PIN = WF5803_SDA_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
////	.CS = 
////	{
////		.GPIO_PORT = WF5803_CS_GPIO_Port,
////		.GPIO_PIN = WF5803_CS_Pin,
////		.defalt_state = GPIO_PIN_SET
////	},
////	.MISO = 
////	{
////		.GPIO_PORT = WF5803_MISO_GPIO_Port,
////		.GPIO_PIN = WF5803_MISO_Pin,
////		.defalt_state = GPIO_PIN_SET
////	},
//	.delay_time = 5
//};//用户层，需要修改的引脚号和间隔时间

ms5837_HandleTypedef sensor;

uint16_t c_temp[8];
uint32_t d1_temp;
uint32_t d2_temp;
int32_t temp;
int32_t p_temp;

uint8_t raw_data[3];


/*Functions*********************************************/
uint16_t readFromMs5837(ms5837_HandleTypedef* sensor,uint8_t add){//看起来像是初始化时用的
	uint8_t dataArr[2] = {0,0};
	IIC_ReadBytes(&i2c2,sensor->addr,add,2,dataArr);
	return (uint16_t)(dataArr[0]<<8)|dataArr[1];
	//uint8_t ret;
	
}

uint8_t crc4(uint16_t n_prom[]) {//应该是校验？
	uint16_t n_rem = 0;

	n_prom[0] = ((n_prom[0]) & 0x0FFF);
	n_prom[7] = 0;

	for ( uint8_t i = 0 ; i < 16; i++ ) {
		if ( i%2 == 1 ) {
			n_rem ^= (uint16_t)((n_prom[i>>1]) & 0x00FF);
		} else {
			n_rem ^= (uint16_t)(n_prom[i>>1] >> 8);
		}
		for ( uint8_t n_bit = 8 ; n_bit > 0 ; n_bit-- ) {
			if ( n_rem & 0x8000 ) {
				n_rem = (n_rem << 1) ^ 0x3000;
			} else {
				n_rem = (n_rem << 1);
			}
		}
	}
	
	n_rem = ((n_rem >> 12) & 0x000F);

	return n_rem ^ 0x00;
}

uint8_t ms5837_init(ms5837_HandleTypedef* sensor, ms5837_type_e type, ms5837_density_e density){//初始化
	sensor->addr = MS5837_ADDR;
	uint16_t inth,intl;
	
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
	(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
	IIC_Send_Byte(&i2c2, CMD_RST);
	(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
	//IIC_Send_Byte(i2c, data_byte);
	(&i2c2)->ack[2]=0;
	IIC_Stop(&i2c2);
	HAL_Delay(10);
	//delay_us(200);
	
	sensor->density = density;
	sensor->model = type;
	
	for ( uint8_t i = 1 ; i < 7 ; i++ ) {
		//c_temp[i] = readFromMs5837(sensor, CMD_PROM_READ+i*2);
		IIC_Start(&i2c2);
		IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
		(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
		IIC_Send_Byte(&i2c2, CMD_PROM_READ + (i * 2));
		(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
		//IIC_Send_Byte(i2c, data_byte);
		(&i2c2)->ack[2]=0;
		IIC_Stop(&i2c2);
		delay_us(5);
		
		IIC_Start(&i2c2);
		IIC_Send_Byte(&i2c2, (sensor->addr)|0x01);
		(&i2c2)->ack[2]=IIC_Wait_Ack(&i2c2);
		//delay_us(10);
		
		delay_us(1);
		inth = IIC_Read_Byte(&i2c2);
		delay_us(1);
		intl = IIC_Read_Byte(&i2c2);
		IIC_Stop(&i2c2);
		
		
		
		
		c_temp[i] = (inth << 8)|(intl >> 8);
	}
	
	uint8_t crcRead = (c_temp[0] & 0xF000) >> 12;
	uint8_t crcCalculated = crc4(c_temp);
	
	//HAL_Delay(10);

	if ( crcCalculated == crcRead ) {
		return 0x01; // Initialization success
	}

	return 0x00; // CRC fail
}

void ms5837_read(ms5837_HandleTypedef* sensor){//读取
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
	(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	IIC_Send_Byte(&i2c2, CMD_CONVERT_D1);
	(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
	//IIC_Send_Byte(i2c, data_byte);
	(&i2c2)->ack[2]=0;
	IIC_Stop(&i2c2);
	delay_us(5000);
	//delay_us(200);
	
	
	//IIC_ReadBytes(&i2c2,sensor->addr,CMD_ADC_READ,3,raw_data);
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
	(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	IIC_Send_Byte(&i2c2, CMD_ADC_READ);
	(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
	//IIC_Send_Byte(i2c, data_byte);
	//(&i2c2)->ack[2]=0;
	IIC_Stop(&i2c2);
	//HAL_Delay(5);
	//delay_us(200);
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x01);
	(&i2c2)->ack[2]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	raw_data[0] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	raw_data[1] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	raw_data[2] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	
//	d1_temp = raw_data[0];
//	d1_temp = d1_temp << 8;
//	d1_temp |= raw_data[1];
//	d1_temp = d1_temp << 8;
//	d1_temp |= raw_data[2];
	d1_temp = raw_data[0] << 16 | raw_data[1] << 8 | raw_data[2];
	IIC_Stop(&i2c2);
	//delay_us(200);
	
	
	
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
	(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	IIC_Send_Byte(&i2c2, CMD_CONVERT_D2);
	(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
	//IIC_Send_Byte(i2c, data_byte);
	(&i2c2)->ack[2]=0;
	IIC_Stop(&i2c2);
	delay_us(5000);
	//delay_us(200);
	
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x00);
	(&i2c2)->ack[0]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	IIC_Send_Byte(&i2c2, CMD_ADC_READ);
	(&i2c2)->ack[1]=IIC_Wait_Ack(&i2c2);
	//IIC_Send_Byte(i2c, data_byte);
	//(&i2c2)->ack[2]=0;
	IIC_Stop(&i2c2);
	//HAL_Delay(20);
	//delay_us(200);
	IIC_Start(&i2c2);
	IIC_Send_Byte(&i2c2, (sensor->addr)|0x01);
	(&i2c2)->ack[2]=IIC_Wait_Ack(&i2c2);
	//delay_us(10);
	raw_data[0] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	raw_data[1] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	raw_data[2] = IIC_Read_Byte(&i2c2);
	//delay_us(10);
	
	//IIC_ReadBytes(&i2c2,sensor->addr,CMD_ADC_READ,3,raw_data);
//	d2_temp = raw_data[0];
//	d2_temp = d2_temp << 8;
//	d2_temp |= raw_data[1];
//	d2_temp = d2_temp << 8;
//	d2_temp |= raw_data[2];
	d2_temp = raw_data[0] << 16 | raw_data[1] << 8 | raw_data[2];
	IIC_Stop(&i2c2);
	//delay_us(200);
	
	
}

void ms5837_calculate(ms5837_HandleTypedef* sensor){//计算
	// Given C1-C6 and D1, D2, calculated TEMP and P
	// Do conversion first and then second order temp compensation
	int32_t dT = 0;
	int64_t SENS = 0;
	int64_t OFF = 0;
	int32_t SENSi = 0;
	int32_t OFFi = 0;  
	int32_t Ti = 0;    
	int64_t OFF2 = 0;
	int64_t SENS2 = 0;
	int64_t Aux = 0;
	
	// Terms called
	dT = d2_temp-((uint32_t)(c_temp[5]))*256;
	if (sensor->model == MS5837_02BA ) {
		SENS = ((int64_t)(c_temp[1]))*65536+(((int64_t)(c_temp[3]))*dT)/128;
		OFF = ((int64_t)(c_temp[2]))*131072+(((int64_t)(c_temp[4]))*dT)/64;
		p_temp = (d1_temp*SENS/(2097152)-OFF)/(32768);
	} else {
		SENS = ((int64_t)(c_temp[1]))*32768+(((int64_t)(c_temp[3]))*dT)/256;
		OFF = ((int64_t)(c_temp[2]))*65536+(((int64_t)(c_temp[4]))*dT)/128;
		p_temp = (d1_temp*SENS/(2097152)-OFF)/(8192);
	}
	
	// Temp conversion
	temp = 2000+((int64_t)(dT))*c_temp[6]/8388608;
	
	//Second order compensation
	if ( sensor->model == MS5837_02BA ) {
		if((temp/100)<20){         //Low temp
			Ti = (11*((int64_t)(dT))*((int64_t)(dT)))/(34359738368);
			OFFi = (31*(temp-2000)*(temp-2000))/8;
			SENSi = (63*(temp-2000)*(temp-2000))/32;
		}
	} else {
		if((temp/100)<20){         //Low temp
			Aux = (temp - 2000) * (temp - 2000);
			Ti = (3*((int64_t)(dT))*((int64_t)(dT)))/(8589934592);
			OFFi = (3*Aux)/2;
			SENSi = (5*Aux)/8;
			if((temp/100)<-15){    //Very low temp
				OFFi = OFFi+7*(temp+1500)*(temp+1500);
				SENSi = SENSi+4*(temp+1500)*(temp+1500);
			}
		}
		else if((temp/100)>=20){    //High temp
			Ti = 2*(dT*dT)/(137438953472);
			OFFi = (1*Aux)/16;
			SENSi = 0;
		}
	}
	
	OFF2 = OFF-OFFi;           //Calculate pressure and temp second order
	SENS2 = SENS-SENSi;
	
	//temp = (temp-Ti) / 100;
	
	if ( sensor->model == MS5837_02BA ) {
		temp = temp - Ti;
		p_temp = (((d1_temp*SENS2)/2097152-OFF2)/32768)/100.0; 
	} else {
		temp = temp - Ti;
		p_temp = (((d1_temp*SENS2)/2097152-OFF2)/8192)/10.0;
	}
}

void ms5837_getpressure(ms5837_HandleTypedef* sensor, float conv){
	if(sensor->model == MS5837_02BA){
		sensor->pressure = p_temp * conv;
	}else{
		sensor->pressure = p_temp * conv;
	}
}

void ms5837_gettemperature(ms5837_HandleTypedef* sensor){
	sensor->temperature = temp;
}

void ms5837_getdepth(ms5837_HandleTypedef* sensor){
	//static float air_pressure = 990.0f;
	ms5837_getpressure(sensor,CONVERT_PA);
	sensor->depth = (((sensor->pressure) - 101300) / ((sensor->density) * 9.80665));
	//sensor->depth = ((sensor->pressure) - air_pressure) * 0.983615;
}