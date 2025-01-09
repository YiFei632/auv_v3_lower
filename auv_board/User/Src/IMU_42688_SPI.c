#include "sim_SPItwo.h"

#define PI 3.1415926

IMU_t IMU = {0}; 

static void Write_Data_ICM42688(uint8_t reg, uint8_t dat);
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num);

// 数据转换为实际物理数据的转换系数
static float icm42688_spi_acc_inv = 1, icm42688_spi_gyro_inv = 1; 

/**
*
* @brief    ICM42688陀螺仪初始化
* @param
* @return   void
* @notes    用户调用
* Example:  Init_ICM42688();
*
**/
void icm42688_init(void)
{
    uint8_t tx_data[2];
    uint8_t whoami;
    
    // 初始化CS引脚为高电平
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    
    // 读取WHO_AM_I寄存器
    tx_data[0] = WHO_AM_I | 0x80;  // 读取操作需要最高位置1
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(&hspi1, tx_data, 1, 100);
		SPI1_Transmit(tx_data, 1, 100);
//    HAL_SPI_Receive(&hspi1, &whoami, 1, 100);
	  SPI1_Receive(&whoami, 1, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    
    IMU.whoimi = whoami;
    HAL_Delay(50);
    
    // 配置PWR_MGMT0寄存器
    tx_data[0] = PWR_MGMT0;
    tx_data[1] = 0x00;
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    
    HAL_Delay(50);
    
    // 设置ICM42688加速度计和陀螺仪的量程和输出速率
    Set_LowpassFilter_Range_ICM42688_SPI(ICM42688_AFS_16G, ICM42688_AODR_32000HZ, 
                                        ICM42688_GFS_2000DPS, ICM42688_GODR_32000HZ);
    HAL_Delay(50);
    
    // 配置PWR_MGMT0寄存器
    tx_data[0] = PWR_MGMT0;
    tx_data[1] = 0x0F;
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    
    HAL_Delay(50);
    
    IMU.is_init = 1;

}



/**
*
* @brief    获得ICM42688陀螺仪加速度
* @param
* @return   void
* @notes    单位:g(m/s^2),用户调用
* Example:  Get_Acc_ICM42688();
*
**/

void Get_Acc_ICM42688(void)
{
    uint8_t buffer[6];
    
    // 使用HAL库读取SPI数据
    Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, buffer, 6);
    
    // 转换为实际加速度值 (mg)
		for(int i = 0; i < 3; i++)
			IMU.acc[i] = icm42688_spi_acc_inv * (int16_t)(((int16_t)buffer[2*i] << 8) | buffer[2*i+1]);
   
}
/**
*
* @brief    获得ICM42688陀螺仪角加速度
* @param
* @return   void
* @notes    单位为:°/s,用户调用
* Example:  Get_Gyro_ICM42688();
*
**/

void Get_Gyro_ICM42688(void)
{
    uint8_t buffer[6];
    
    // 使用HAL库读取SPI数据
    Read_Datas_ICM42688(ICM42688_GYRO_DATA_X1, buffer, 6);
    
    // 转换为实际角速度值 (dps)
   for(int i = 0; i < 3; i++)
    IMU.gyro[i] = icm42688_spi_gyro_inv * (int16_t)(((int16_t)buffer[2*i] << 8) | buffer[2*i+1]);
}

void Get_ICM42688_Data()
{
//	uint8_t temp = 0;
//	uint16_t Counting_Temp = 0;
	
	Get_Acc_ICM42688();
	Get_Gyro_ICM42688();
	Angle_Calcu();
	for(int i = 0;i < 3;i++){
		if(IMU.angle[i] > 180.0f){
			IMU.angle[i] = IMU.angle[i] - 360.0f * floor((IMU.angle[i] + 180.0f) / 360.0f);
		}else if(IMU.angle[i] < -180.0f){
			IMU.angle[i] = IMU.angle[i] + 360.0f * ceil((IMU.angle[i] - 180.0f) / 360.0f);
		}
		
		IMU.angle[i] = (IMU.angle[i] / 360.0f) * 2 * PI;
	}
}

/**
*
* @brief    设置ICM42688陀螺仪低通滤波器带宽和量程
* @param    afs                 // 加速度计量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    aodr                // 加速度计输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @param    gfs                 // 陀螺仪量程,可在dmx_icm42688.h文件里枚举定义中查看
* @param    godr                // 陀螺仪输出速率,可在dmx_icm42688.h文件里枚举定义中查看
* @return   void
* @notes    ICM42688.c文件内部调用,用户无需调用尝试
* Example:  Set_LowpassFilter_Range_ICM42688_SPI(ICM42688_AFS_16G,ICM42688_AODR_32000HZ,ICM42688_GFS_2000DPS,ICM42688_GODR_32000HZ);
*
**/
void Set_LowpassFilter_Range_ICM42688_SPI(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr)
{
    uint8_t tx_data[2];
    
    // 写入加速度计配置
    tx_data[0] = ACCEL_CONFIG0;  // 地址
    tx_data[1] = (afs << 5) | (aodr + 1);  // 数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);  // 发送配置
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高
    
    // 写入陀螺仪配置
    tx_data[0] = GYRO_CONFIG0;   // 地址
    tx_data[1] = (gfs << 5) | (godr + 1);  // 数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);  // 发送配置
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高

    // 设置量程转换系数
    switch(afs)
    {
    case ICM42688_AFS_2G:
        icm42688_spi_acc_inv = 2 / 32768.0f;             
        break;
    case ICM42688_AFS_4G:
        icm42688_spi_acc_inv = 4 / 32768.0f;             
        break;
    case ICM42688_AFS_8G:
        icm42688_spi_acc_inv = 8 / 32768.0f;             
        break;
    case ICM42688_AFS_16G:
        icm42688_spi_acc_inv = 16 / 32768.0f;            
        break;
    default:
        icm42688_spi_acc_inv = 1;                           
        break;
    }
    
    switch(gfs)
    {
    case ICM42688_GFS_15_625DPS:
        icm42688_spi_gyro_inv = 15.625f / 32768.0f;         
        break;
    case ICM42688_GFS_31_25DPS:
        icm42688_spi_gyro_inv = 31.25f / 32768.0f;          
        break;
    case ICM42688_GFS_62_5DPS:
        icm42688_spi_gyro_inv = 62.5f / 32768.0f;           
        break;
    case ICM42688_GFS_125DPS:
        icm42688_spi_gyro_inv = 125.0f / 32768.0f;          
        break;
    case ICM42688_GFS_250DPS:
        icm42688_spi_gyro_inv = 250.0f / 32768.0f;          
        break;
    case ICM42688_GFS_500DPS:
        icm42688_spi_gyro_inv = 500.0f / 32768.0f;          
        break;
    case ICM42688_GFS_1000DPS:
        icm42688_spi_gyro_inv = 1000.0f / 32768.0f;         
        break;
    case ICM42688_GFS_2000DPS:
        icm42688_spi_gyro_inv = 2000.0f / 32768.0f;         
        break;
    default:
        icm42688_spi_gyro_inv = 1;                          
        break;
    }
}



// 写入寄存器数据
static void Write_Data_ICM42688(uint8_t reg, uint8_t dat)
{
    uint8_t send_data[2];
    send_data[0] = reg & 0x7F;    // 清除最高位，表示写操作
    send_data[1] = dat;

    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低，开始通信
    
    SPI1_Transmit(send_data, 2, HAL_MAX_DELAY);  // 发送寄存器地址和数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高，结束通信
}

// 读取多个寄存器数据
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num)
{
    reg |= 0x80;    // 设置最高位，表示读操作
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低，开始通信
    
    SPI1_Transmit(&reg, 1, HAL_MAX_DELAY);      // 发送寄存器地址
    SPI1_Receive(dat, num, HAL_MAX_DELAY);      // 读取数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高，结束通信
}





