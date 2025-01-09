#include "tlv493d.h"

TLV493D_t TLV493D;


//sim_I2C_t i2c4 = 
//{
//	.SCL = 
//	{
//		.GPIO_PORT = MLX90393_SCL_GPIO_Port,
//		.GPIO_PIN = MLX90393_SCL_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
//	.SDA = 
//	{
//		.GPIO_PORT = MLX90393_SDA_GPIO_Port,
//		.GPIO_PIN = MLX90393_SDA_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
////	.CS = 
////	{
////		.GPIO_PORT = ICM42688_CS_GPIO_Port,
////		.GPIO_PIN = ICM42688_CS_Pin,
////		.defalt_state = GPIO_PIN_SET
////	},
////	.MISO = 
////	{
////		.GPIO_PORT = ICM42688_MISO_GPIO_Port,
////		.GPIO_PIN = ICM42688_MISO_Pin,
////		.defalt_state = GPIO_PIN_SET
////	},
//	.delay_time = 4
//};//用户层，需要修改的引脚号和间隔时间

//uint8_t cmd_tlv493d[3] = {0x61, 0x00, 0x40}; // 01100001   data_tlv493d[8]     
//uint8_t data_tlv493d[10];
//void tlv493d_init()
//{
//	HAL_GPIO_WritePin	(i2c4.SCL.GPIO_PORT, i2c4.SCL.GPIO_PIN, i2c4.SCL.defalt_state);
//	HAL_GPIO_WritePin	(i2c4.SDA.GPIO_PORT, i2c4.SDA.GPIO_PIN, i2c4.SDA.defalt_state);

//	IIC_Reset_TLV493D(&i2c4, DeviceAddress_TLV493D_3);
//	HAL_Delay(10);
//	IIC_ReadBytes(&i2c4, DeviceAddress_TLV493D_3, 0x00, sizeof(data_tlv493d), data_tlv493d);
//	cmd_tlv493d[1] = data_tlv493d[8];
//	IIC_WriteBytes(&i2c4, DeviceAddress_TLV493D_3, 0x00, sizeof(cmd_tlv493d), cmd_tlv493d);
//	HAL_Delay(100);
//	if(i2c4.ack[0] ==0 && i2c4.ack[9] == 0)
//		TLV493D.is_init = 1;
//	else
//		TLV493D.error_code = TLV493D_Ack_Error;
//}

//void Decode_TLV493D_Data(uint8_t * data)
//{
//	if((data[5] & 0x20) != 0x20)
//	{
//		TLV493D.error_code = TLV493D_Response_Error;
//		return;
//	}
//	
//	TLV493D.data[0] = ( data[0] << 4 ) | (((data[4] & 0xF0) >> 4) & 0x0F);
//	TLV493D.data[1] = ( data[1] << 4 ) | (data[4] & 0x0F);
//	TLV493D.data[2] = ( data[2] << 4 ) | (data[5] & 0x0F);
//	TLV493D.data[3] = ( (data[3] & 0xF0) << 4 ) | data[6];
//	for(int k = 0; k < 3; k++)
//	{
//		if(TLV493D.data[k] > 2047)
//			TLV493D.data[k] -= 4096;
//		TLV493D.B[k] = TLV493D.data[k] * 0.098;
//	}
//	if(TLV493D.data[3] > 1023)
//			TLV493D.data[3] -= 2048;
//	TLV493D.temperature = (TLV493D.data[3] - 340) * 1.1 + 25.;
//}

//void Get_TLV493D_Data()
//{
//	IIC_ReadBytes(&i2c4, DeviceAddress_TLV493D_0, 0x00, sizeof(data_tlv493d), data_tlv493d);
//	Decode_TLV493D_Data(data_tlv493d);
//}