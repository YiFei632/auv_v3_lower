#include "icm_42688.h"

uint8_t ICM_42688_ADDR_AD0_LOW_READ = (0x68 << 1) | 0x01;   //AD0低电平地址的读 0xD1
uint8_t ICM_42688_ADDR_AD0_HIGH_READ = (0x69 << 1) | 0x01;	 //AD0高电平地址的读 0xD3
uint8_t ICM_42688_ADDR_AD0_LOW_WRITE = (0x68 << 1) | 0x00;	 //AD0低电平地址的写 0xD0
uint8_t ICM_42688_ADDR_AD0_HIGH_WRITE = (0x69 << 1) | 0x00; //AD0高电平地址的写 0xD2

sim_I2C_t i2c1 = 
{
	.SCL = 
	{
		.GPIO_PORT = ICM42688_SCL_GPIO_Port,
		.GPIO_PIN = ICM42688_SCL_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.SDA = 
	{
		.GPIO_PORT = ICM42688_SDA_GPIO_Port,
		.GPIO_PIN = ICM42688_SDA_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.CS = 
	{
		.GPIO_PORT = ICM42688_CS_GPIO_Port,
		.GPIO_PIN = ICM42688_CS_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.MISO = 
	{
		.GPIO_PORT = ICM42688_MISO_GPIO_Port,
		.GPIO_PIN = ICM42688_MISO_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.delay_time = 1
};//用户层，需要修改的引脚号和间隔时间


IMU_t IMU1 = {0};

// 数据转换为实际物理数据的转换系数
static float icm42688_iic_acc_inv = 1, icm42688_iic_gyro_inv = 1;
/**
*
* @brief    设置ICM42688陀螺仪低通滤波器带宽和量程
* @param    afs                 // 加速度计量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    aodr                // 加速度计输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @param    gfs                 // 陀螺仪量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    godr                // 陀螺仪输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Set_LowpassFilter_Range_ICM42688_IIC(ICM42688_AFS_16G,ICM42688_AODR_32000HZ,ICM42688_GFS_2000DPS,ICM42688_GODR_32000HZ);
*
**/
void Set_LowpassFilter_Range_ICM42688_IIC(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr)
{
	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, ACCEL_CONFIG0, (afs << 5) | (aodr + 1));   // 初始化ACCEL量程和输出速率(p77)
	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, GYRO_CONFIG0, (gfs << 5) | (godr + 1));    // 初始化GYRO量程和输出速率(p76)

	switch(afs)
	{
	case ICM42688_AFS_2G:
			icm42688_iic_acc_inv = 2 / 32768.0f;             // 加速度计量程为:±2g
			break;
	case ICM42688_AFS_4G:
			icm42688_iic_acc_inv = 4 / 32768.0f;             // 加速度计量程为:±4g
			break;
	case ICM42688_AFS_8G:
			icm42688_iic_acc_inv = 8 / 32768.0f;             // 加速度计量程为:±8g
			break;
	case ICM42688_AFS_16G:
			icm42688_iic_acc_inv = 16 / 32768.0f;            // 加速度计量程为:±16g
			break;
	default:
			icm42688_iic_acc_inv = 1;                           // 不转化为实际数据
			break;
	}
	switch(gfs)
	{
	case ICM42688_GFS_15_625DPS:
			icm42688_iic_gyro_inv = 15.625f / 32768.0f;         // 陀螺仪量程为:±15.625dps
			break;
	case ICM42688_GFS_31_25DPS:
			icm42688_iic_gyro_inv = 31.25f / 32768.0f;          // 陀螺仪量程为:±31.25dps
			break;
	case ICM42688_GFS_62_5DPS:
			icm42688_iic_gyro_inv = 62.5f / 32768.0f;           // 陀螺仪量程为:±62.5dps
			break;
	case ICM42688_GFS_125DPS:
			icm42688_iic_gyro_inv = 125.0f / 32768.0f;          // 陀螺仪量程为:±125dps
			break;
	case ICM42688_GFS_250DPS:
			icm42688_iic_gyro_inv = 250.0f / 32768.0f;          // 陀螺仪量程为:±250dps
			break;
	case ICM42688_GFS_500DPS:
			icm42688_iic_gyro_inv = 500.0f / 32768.0f;          // 陀螺仪量程为:±500dps
			break;
	case ICM42688_GFS_1000DPS:
			icm42688_iic_gyro_inv = 1000.0f / 32768.0f;         // 陀螺仪量程为:±1000dps
			break;
	case ICM42688_GFS_2000DPS:
			icm42688_iic_gyro_inv = 2000.0f / 32768.0f;         // 陀螺仪量程为:±2000dps
			break;
	default:
			icm42688_iic_gyro_inv = 1;                          // 不转化为实际数据
			break;
	}
}   

void icm42688_init()
{
	HAL_GPIO_WritePin	(i2c1.SCL.GPIO_PORT, i2c1.SCL.GPIO_PIN, i2c1.SCL.defalt_state);
	HAL_GPIO_WritePin	(i2c1.SDA.GPIO_PORT, i2c1.SDA.GPIO_PIN, i2c1.SDA.defalt_state);
	HAL_GPIO_WritePin	(i2c1.CS.GPIO_PORT, i2c1.CS.GPIO_PIN, i2c1.CS.defalt_state);
	HAL_GPIO_WritePin	(i2c1.MISO.GPIO_PORT, i2c1.MISO.GPIO_PIN, i2c1.MISO.defalt_state);
	HAL_Delay(10);
	IMU1.whoimi = IIC_ReadOneByte(&i2c1, DeviceAddress_ICM42688, WHO_AM_I);
	HAL_Delay(50);
	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, PWR_MGMT0, 0x00);
	HAL_Delay(50);
// 设置ICM42688加速度计和陀螺仪的量程和输出速率
	Set_LowpassFilter_Range_ICM42688_IIC(ICM42688_AFS_16G, ICM42688_AODR_32000HZ, ICM42688_GFS_2000DPS, ICM42688_GODR_32000HZ);
	HAL_Delay(50);
	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, PWR_MGMT0, 0x0F);
	HAL_Delay(50);
	IMU1.is_init = 1;
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, DEVICE_CONFIG, 0x00);//Software reset configuration and SPI mode selection
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, DRIVE_CONFIG, 0x05);//Control the communication speed(I guess)
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, INT_CONFIG, 0x02);//interrupt settings
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, PWR_MGMT0, 0x0F);//power register of sensors(it won't working if we don't turn it on)
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, INT_CONFIG1,0x00);//this register is to set the interrupt port's Interrupt pulse duration (more details on datasheet)
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, INT_SOURCE0,0x08);//setting interrupt port's interrupt source
//	HAL_Delay(50);
//	//陀螺仪初始化
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, GYRO_CONFIG0, 0x06);//调整采样率和ODR
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, GYRO_CONFIG1, 0x16);//调整带宽和滤波次数
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, GYRO_ACCEL_CONFIG0, 0x11);//调整陀螺仪和加速度计的低通滤波器带宽
//	HAL_Delay(50);
//	//加速度计初始化
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, ACCEL_CONFIG0, 0x06);//调整采样率和ODR
//	HAL_Delay(50);
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, ACCEL_CONFIG1, 0x0D);//调整带宽和滤波次数
//	HAL_Delay(50);
//	//自检
//	IIC_WriteOneByte(&i2c1, DeviceAddress_ICM42688, SELF_TEST_CONFIG, 0x1F);//调整带宽和滤波次数
//	HAL_Delay(50);
}

/**
*
* @brief    获得ICM42688陀螺仪加速度
* @param
* @return   void
* @notes    单位:g(m/s^2),用户调用
* Example:  Get_Acc_ICM42688_IIC();
*
**/
void Get_Gyro_ICM42688_IIC(void)
{
    uint8_t data[6];
	IIC_ReadBytes(&i2c1, DeviceAddress_ICM42688, GYRO_DATA_X1, 6, data);
	for(int i = 0; i < 3; i++)
    IMU1.gyro[i] = icm42688_iic_gyro_inv * (int16_t)(((int16_t)data[2*i] << 8) | data[2*i+1]);
}

/**
*
* @brief    获得ICM42688陀螺仪角加速度
* @param
* @return   void
* @notes    单位为:°/s,用户调用
* Example:  Get_Gyro_ICM42688_IIC();
*
**/
void Get_Acc_ICM42688_IIC(void)
{
    uint8_t data[6];
	IIC_ReadBytes(&i2c1, DeviceAddress_ICM42688, ACCEL_DATA_X1, 6, data);
	for(int i = 0; i < 3; i++)
    IMU1.acc[i] = icm42688_iic_acc_inv * (int16_t)(((int16_t)data[2*i] << 8) | data[2*i+1]);
}

void Get_ICM42688_Data()
{
	uint8_t temp = 0;
	uint16_t Counting_Temp = 0;
	
	Get_Gyro_ICM42688_IIC();
	Get_Acc_ICM42688_IIC();
	Angle_Calcu();
//	for(int i=0;i<3;i++){
//		IMU1.angle[i]=(IMU1.angle[i]/360.0f)*(2*3.1415926f);
//	}
}