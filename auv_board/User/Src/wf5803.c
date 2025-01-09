#include "wf5803.h"

sim_I2C_t i2c2 = 
{
	.SCL = 
	{
		.GPIO_PORT = WF5803_SCL_GPIO_Port,
		.GPIO_PIN = WF5803_SCL_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.SDA = 
	{
		.GPIO_PORT = WF5803_SDA_GPIO_Port,
		.GPIO_PIN = WF5803_SDA_Pin,
		.defalt_state = GPIO_PIN_SET
	},
//	.CS = 
//	{
//		.GPIO_PORT = WF5803_CS_GPIO_Port,
//		.GPIO_PIN = WF5803_CS_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
//	.MISO = 
//	{
//		.GPIO_PORT = WF5803_MISO_GPIO_Port,
//		.GPIO_PIN = WF5803_MISO_Pin,
//		.defalt_state = GPIO_PIN_SET
//	},
	.delay_time = 5
};//�û��㣬��Ҫ�޸ĵ����źźͼ��ʱ��

WF5803_t WF5803 = {0};

void wf5803_init()
{
	HAL_GPIO_WritePin	(i2c2.SCL.GPIO_PORT, i2c2.SCL.GPIO_PIN, i2c2.SCL.defalt_state);
	HAL_GPIO_WritePin	(i2c2.SDA.GPIO_PORT, i2c2.SDA.GPIO_PIN, i2c2.SDA.defalt_state);
//	HAL_GPIO_WritePin	(i2c2.CS.GPIO_PORT, i2c2.CS.GPIO_PIN, i2c2.CS.defalt_state);
//	HAL_GPIO_WritePin	(i2c2.MISO.GPIO_PORT, i2c2.MISO.GPIO_PIN, i2c2.MISO.defalt_state);
	HAL_Delay(10);
	IIC_WriteOneByte(&i2c2, DeviceAddress_WF5803, WF5803_CMD_ADDR, WF5803_CMD_CONTINEOUS);
	HAL_Delay(100);
	
	// ���ж�β����Լ�������е�ѹǿ��Ϊˮ�����λ
	float total_pressure = 0.0f;
	for (int i = 0; i < PRESSURE_MEASURE_COUNT; i++)
	{
			wf5803_getdata();  // ��ȡѹ�����ݲ����� WF5803.pressure ��ֵ
			total_pressure += WF5803.pressure;
			HAL_Delay(50);  // �ӳ���ȷ��ÿ�β���֮�����㹻��ʱ����
	}
	WF5803.air_pressure = total_pressure / PRESSURE_MEASURE_COUNT;  // ����ƽ��ѹ����������Ϊˮ�����λ
	WF5803.is_init = 1;
}

void wf5803_getstatus()
{
	uint8_t status = IIC_ReadOneByte(&i2c2, DeviceAddress_WF5803, WF5803_STATUS_ADDR);
	WF5803.is_ready = status & 0x01;
	WF5803.error_code = (status & 0xF0) >> 4;
}

void wf5803_getdata()
{
//	wf5803_getstatus();
	
	uint8_t raw_data[5];
	uint32_t temp;
	double temp_f;
	IIC_ReadBytes(&i2c2, DeviceAddress_WF5803, WF5803_DATA_ADDR, 5, raw_data);
	temp = raw_data[0];
  temp = temp << 8;
  temp |= raw_data[1];
  temp = temp << 8;
  temp |= raw_data[2];
  // 2020-06-08 v2�궨��ʽ
  if (temp > 8388608)
  {
    temp_f = (temp - 16777216) / 8388608.0f;
  }
  else
  {
    temp_f = temp / 8388608.0f;
  }
  WF5803.pressure  = 125 * (temp_f - 0.1f) + 30;
  //
  temp = raw_data[3];
  temp = temp << 8;
  temp |= raw_data[4];
  if (temp > 32768)
  {
    WF5803.temperature = (temp - 65536) / 256.0f;
  }
  else
  {
    WF5803.temperature = temp / 256.0f;
  }
	
	if(WF5803.is_init)
		WF5803.depth = (WF5803.pressure - WF5803.air_pressure) / 9.8; //p=ro*g*h p��λkpa 
}