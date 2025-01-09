#include "sim_SPItwo.h"

#define PI 3.1415926

IMU_t IMU = {0}; 

static void Write_Data_ICM42688(uint8_t reg, uint8_t dat);
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num);

// ����ת��Ϊʵ���������ݵ�ת��ϵ��
static float icm42688_spi_acc_inv = 1, icm42688_spi_gyro_inv = 1; 

/**
*
* @brief    ICM42688�����ǳ�ʼ��
* @param
* @return   void
* @notes    �û�����
* Example:  Init_ICM42688();
*
**/
void icm42688_init(void)
{
    uint8_t tx_data[2];
    uint8_t whoami;
    
    // ��ʼ��CS����Ϊ�ߵ�ƽ
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(10);
    
    // ��ȡWHO_AM_I�Ĵ���
    tx_data[0] = WHO_AM_I | 0x80;  // ��ȡ������Ҫ���λ��1
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(&hspi1, tx_data, 1, 100);
		SPI1_Transmit(tx_data, 1, 100);
//    HAL_SPI_Receive(&hspi1, &whoami, 1, 100);
	  SPI1_Receive(&whoami, 1, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    
    IMU.whoimi = whoami;
    HAL_Delay(50);
    
    // ����PWR_MGMT0�Ĵ���
    tx_data[0] = PWR_MGMT0;
    tx_data[1] = 0x00;
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
    
    HAL_Delay(50);
    
    // ����ICM42688���ٶȼƺ������ǵ����̺��������
    Set_LowpassFilter_Range_ICM42688_SPI(ICM42688_AFS_16G, ICM42688_AODR_32000HZ, 
                                        ICM42688_GFS_2000DPS, ICM42688_GODR_32000HZ);
    HAL_Delay(50);
    
    // ����PWR_MGMT0�Ĵ���
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
* @brief    ���ICM42688�����Ǽ��ٶ�
* @param
* @return   void
* @notes    ��λ:g(m/s^2),�û�����
* Example:  Get_Acc_ICM42688();
*
**/

void Get_Acc_ICM42688(void)
{
    uint8_t buffer[6];
    
    // ʹ��HAL���ȡSPI����
    Read_Datas_ICM42688(ICM42688_ACCEL_DATA_X1, buffer, 6);
    
    // ת��Ϊʵ�ʼ��ٶ�ֵ (mg)
		for(int i = 0; i < 3; i++)
			IMU.acc[i] = icm42688_spi_acc_inv * (int16_t)(((int16_t)buffer[2*i] << 8) | buffer[2*i+1]);
   
}
/**
*
* @brief    ���ICM42688�����ǽǼ��ٶ�
* @param
* @return   void
* @notes    ��λΪ:��/s,�û�����
* Example:  Get_Gyro_ICM42688();
*
**/

void Get_Gyro_ICM42688(void)
{
    uint8_t buffer[6];
    
    // ʹ��HAL���ȡSPI����
    Read_Datas_ICM42688(ICM42688_GYRO_DATA_X1, buffer, 6);
    
    // ת��Ϊʵ�ʽ��ٶ�ֵ (dps)
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
* @brief    ����ICM42688�����ǵ�ͨ�˲������������
* @param    afs                 // ���ٶȼ�����,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    aodr                // ���ٶȼ��������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    gfs                 // ����������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @param    godr                // �������������,����dmx_icm42688.h�ļ���ö�ٶ����в鿴
* @return   void
* @notes    ICM42688.c�ļ��ڲ�����,�û�������ó���
* Example:  Set_LowpassFilter_Range_ICM42688_SPI(ICM42688_AFS_16G,ICM42688_AODR_32000HZ,ICM42688_GFS_2000DPS,ICM42688_GODR_32000HZ);
*
**/
void Set_LowpassFilter_Range_ICM42688_SPI(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr)
{
    uint8_t tx_data[2];
    
    // д����ٶȼ�����
    tx_data[0] = ACCEL_CONFIG0;  // ��ַ
    tx_data[1] = (afs << 5) | (aodr + 1);  // ����
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS����
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);  // ��������
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS����
    
    // д������������
    tx_data[0] = GYRO_CONFIG0;   // ��ַ
    tx_data[1] = (gfs << 5) | (godr + 1);  // ����
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS����
//    HAL_SPI_Transmit(&hspi1, tx_data, 2, 100);  // ��������
		SPI1_Transmit(tx_data, 2, 100);
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS����

    // ��������ת��ϵ��
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



// д��Ĵ�������
static void Write_Data_ICM42688(uint8_t reg, uint8_t dat)
{
    uint8_t send_data[2];
    send_data[0] = reg & 0x7F;    // ������λ����ʾд����
    send_data[1] = dat;

    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS���ͣ���ʼͨ��
    
    SPI1_Transmit(send_data, 2, HAL_MAX_DELAY);  // ���ͼĴ�����ַ������
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS���ߣ�����ͨ��
}

// ��ȡ����Ĵ�������
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num)
{
    reg |= 0x80;    // �������λ����ʾ������
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS���ͣ���ʼͨ��
    
    SPI1_Transmit(&reg, 1, HAL_MAX_DELAY);      // ���ͼĴ�����ַ
    SPI1_Receive(dat, num, HAL_MAX_DELAY);      // ��ȡ����
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS���ߣ�����ͨ��
}





