#ifndef __ICM42688_SPI_H
#define __ICM42688_SPI_H

#include "stm32h7xx_hal.h"
#include "main.h"
#include "kalmanfilter.h"


/**USER BANK 0 REGISTER MAP – DESCRIPTIONS**/
#define DEVICE_CONFIG 0X11		//Serial IF: R/W	 Reset value: 0x00
#define DRIVE_CONFIG  0X13		//Serial IF: R/W   Reset value: 0x05
#define INT_CONFIG    0X14		//Serial IF: R/W	 Reset value: 0x00
#define FIFO_CONFIG 	0X16		//Serial IF: R/W	 Reset value: 0x00
#define TEMP_DATA1	  0X1D		//Serial IF: SYNCR Reset value: 0x80
#define TEMP_DATA0 		0X1E		//Serial IF: SYNCR Reset value: 0x00

#define ACCEL_DATA_X1 0X1F		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_X0 0X20		//Serial IF: SYNCR Reset value: 0x00
#define ACCEL_DATA_Y1 0X21		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_Y0 0X22		//Serial IF: SYNCR Reset value: 0x00
#define ACCEL_DATA_Z1 0X23		//Serial IF: SYNCR Reset value: 0x80
#define ACCEL_DATA_Z0 0X24		//Serial IF: SYNCR Reset value: 0x00

#define GYRO_DATA_X1  0X25		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_X0  0X26		//Serial IF: SYNCR Reset value: 0x00
#define GYRO_DATA_Y1  0X27		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_Y0  0X28		//Serial IF: SYNCR Reset value: 0x00
#define GYRO_DATA_Z1  0X29		//Serial IF: SYNCR Reset value: 0x80
#define GYRO_DATA_Z0  0X2A		//Serial IF: SYNCR Reset value: 0x00

#define TMST_FSYNCH   0X2B		//Serial IF: SYNCR Reset value: 0x00
#define TMST_FSYNCL   0X2C    //Serial IF: SYNCR Reset value: 0x00
#define INT_STATUS    0X2D    //Serial IF: R/C   Reset value: 0x10
#define FIFO_COUNTH   0X2E    //Serial IF: R     Reset value: 0x00
#define FIFO_COUNTL   0X2F    //Serial IF: R     Reset value: 0x00
#define FIFO_DATA     0X30    //Serial IF: R     Reset value: 0xFF
#define APEX_DATA0    0X31    //Serial IF: SYNCR Reset value: 0x00
#define APEX_DATA1    0X32    //Serial IF: SYNCR Reset value: 0x00
#define APEX_DATA2    0X33    //Serial IF: R     Reset value: 0x00
#define APEX_DATA3    0X34    //Serial IF: R     Reset value: 0x04
#define APEX_DATA4    0X35    //Serial IF: R     Reset value: 0x00
#define APEX_DATA5    0X36    //Serial IF: R     Reset value: 0x00
#define INT_STATUS2   0X37    //Serial IF: R/C   Reset value: 0x00
#define INT_STATUS3   0X38    //Serial IF: R/C   Reset value: 0x00

#define SIGNAL_PATH_RESET   0X4B    //Serial IF: W/C   Reset value: 0x00

#define INTF_CONFIG0  0X4C    //Serial IF: R/W   Reset value: 0x30
#define INTF_CONFIG1  0X4D    //Serial IF: R/W   Reset value: 0x91
#define PWR_MGMT0     0X4E    //Serial IF: R/W   Reset value: 0x00
#define GYRO_CONFIG0  0X4F    //Serial IF: R/W   Reset value: 0x06
#define ACCEL_CONFIG0 0X50    //Serial IF: R/W   Reset value: 0x06
#define GYRO_CONFIG1  0X51    //Serial IF: R/W   Reset value: 0x16

#define GYRO_ACCEL_CONFIG0   0X52    //Serial IF: R/W   Reset value: 0x11

#define ACCEL_CONFIG1 0X53    //Serial IF: R/W   Reset value: 0x0D
#define TMST_CONFIG   0X54    //Serial IF: R/W   Reset value: 0x23
#define APEX_CONFIG0  0X56    //Serial IF: R/W   Reset value: 0x82
#define SMD_CONFIG    0X57    //Serial IF: R/W   Reset value: 0x00
#define FIFO_CONFIG1  0X5F    //Serial IF: R/W   Reset value: 0x00
#define FIFO_CONFIG2  0X60    //Serial IF: R/W   Reset value: 0x00
#define FIFO_CONFIG3  0X61    //Serial IF: R/W   Reset value: 0x00
#define FSYNC_CONFIG  0X62    //Serial IF: R/W   Reset value: 0x10
#define INT_CONFIG0   0X63    //Serial IF: R/W   Reset value: 0x00
#define INT_CONFIG1   0X64    //Serial IF: R/W   Reset value: 0x10
#define INT_SOURCE0   0X65    //Serial IF: R/W   Reset value: 0x10
#define INT_SOURCE1   0X66    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE3   0X68    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE4   0X69    //Serial IF: R/W   Reset value: 0x00

#define FIFO_LOST_PKT0   0X6C    //Serial IF: R   Reset value: 0x00

#define FIFO_LOST_PKT1   0X6D    //Serial IF: R   Reset value: 0x00
#define SELF_TEST_CONFIG   0X70    //Serial IF: R/W   Reset value: 0x00
#define WHO_AM_I   0X75   //Serial IF: R   Reset value: 0x47
#define REG_BANK_SEL   0X76    //Serial IF: R/W   Reset value: 0x00


/**USER BANK 1 REGISTER MAP – DESCRIPTIONS**/
#define SENSOR_CONFIG0   0X03    //Serial IF: R/W   Reset value: 0x80
#define GYRO_CONFIG_STATIC2   0X0B    //Serial IF: R/W   Reset value: 0xA0
#define GYRO_CONFIG_STATIC3   0X0C    //Serial IF: R/W   Reset value: 0x0D
#define GYRO_CONFIG_STATIC4   0X0D    //Serial IF: R/W   Reset value: 0xAA
#define GYRO_CONFIG_STATIC5   0X0E    //Serial IF: R/W   Reset value: 0x80
#define GYRO_CONFIG_STATIC6   0X0F    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
#define GYRO_CONFIG_STATIC7   0X10    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
#define GYRO_CONFIG_STATIC8   0X11    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
#define GYRO_CONFIG_STATIC9   0X12    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
#define GYRO_CONFIG_STATIC10  0X13    //Serial IF: R/W   Reset value: 0x11
#define XG_ST_DATA   0X5F    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
#define YG_ST_DATA   0X60    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
#define ZG_ST_DATA   0X61    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
#define TMSTVAL0   0X62    //Serial IF: R   Reset value: 0x00
#define TMSTVAL1   0X63    //Serial IF: R   Reset value: 0x00
#define TMSTVAL2   0X64    //Serial IF: R   Reset value: 0x00

#define INTF_CONFIG4   0X7A    //Serial IF: R/W   Reset value: 0x83
#define INTF_CONFIG5   0X7B    //Serial IF: R/W   Reset value: 0x00
#define INTF_CONFIG6   0X7C    //Serial IF: R/W   Reset value: 0x5F

/**USER BANK 2 REGISTER MAP – DESCRIPTIONS**/
#define ACCEL_CONFIG_STATIC2   0X03    //Serial IF: R/W   Reset value: 0x30
#define ACCEL_CONFIG_STATIC3   0X04    //Serial IF: R/W   Reset value: 0x40
#define ACCEL_CONFIG_STATIC4   0X05    //Serial IF: R/W   Reset value: 0x62
#define XA_ST_DATA   0X3B    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
#define YA_ST_DATA   0X3C    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
#define ZA_ST_DATA   0X3D    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)

/**USER BANK 4 REGISTER MAP – DESCRIPTIONS**/
#define APEX_CONFIG1   0X40    //Serial IF: R/W   Reset value: 0xA2
#define APEX_CONFIG2   0X41    //Serial IF: R/W   Reset value: 0x85
#define APEX_CONFIG3   0X42    //Serial IF: R/W   Reset value: 0x51
#define APEX_CONFIG4   0X43    //Serial IF: R/W   Reset value: 0xA4
#define APEX_CONFIG5   0X44    //Serial IF: R/W   Reset value: 0x8C
#define APEX_CONFIG6   0X45    //Serial IF: R/W   Reset value: 0x5C
#define APEX_CONFIG7   0X46    //Serial IF: R/W   Reset value: 0x45
#define APEX_CONFIG8   0X47    //Serial IF: R/W   Reset value: 0x5B
#define APEX_CONFIG9   0X48    //Serial IF: R/W   Reset value: 0x00
#define ACCEL_WOM_X_THR   0X4A    //Serial IF: R/W   Reset value: 0x00
#define ACCEL_WOM_Y_THR   0X4B    //Serial IF: R/W   Reset value: 0x00
#define ACCEL_WOM_Z_THR   0X4C    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE6   0X4D    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE7   0X4E    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE8   0X4F    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE9   0X50    //Serial IF: R/W   Reset value: 0x00
#define INT_SOURCE10  0X51    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER0  0X77    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER1  0X78    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER2  0X79    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER3  0X7A    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER4  0X7B    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER5  0X7C    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER6  0X7D    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER7  0X7E    //Serial IF: R/W   Reset value: 0x00
#define OFFSET_USER8  0X7F    //Serial IF: R/W   Reset value: 0x00

#define ICM42688_DEVICE_CONFIG             0x11
#define ICM42688_DRIVE_CONFIG              0x13
#define ICM42688_INT_CONFIG                0x14
#define ICM42688_FIFO_CONFIG               0x16
#define ICM42688_TEMP_DATA1                0x1D
#define ICM42688_TEMP_DATA0                0x1E
#define ICM42688_ACCEL_DATA_X1             0x1F
#define ICM42688_ACCEL_DATA_X0             0x20
#define ICM42688_ACCEL_DATA_Y1             0x21
#define ICM42688_ACCEL_DATA_Y0             0x22
#define ICM42688_ACCEL_DATA_Z1             0x23
#define ICM42688_ACCEL_DATA_Z0             0x24
#define ICM42688_GYRO_DATA_X1              0x25
#define ICM42688_GYRO_DATA_X0              0x26
#define ICM42688_GYRO_DATA_Y1              0x27
#define ICM42688_GYRO_DATA_Y0              0x28
#define ICM42688_GYRO_DATA_Z1              0x29
#define ICM42688_GYRO_DATA_Z0              0x2A
#define ICM42688_TMST_FSYNCH               0x2B
#define ICM42688_TMST_FSYNCL               0x2C
#define ICM42688_INT_STATUS                0x2D
#define ICM42688_FIFO_COUNTH               0x2E
#define ICM42688_FIFO_COUNTL               0x2F
#define ICM42688_FIFO_DATA                 0x30
#define ICM42688_APEX_DATA0                0x31
#define ICM42688_APEX_DATA1                0x32
#define ICM42688_APEX_DATA2                0x33
#define ICM42688_APEX_DATA3                0x34
#define ICM42688_APEX_DATA4                0x35
#define ICM42688_APEX_DATA5                0x36
#define ICM42688_INT_STATUS2               0x37
#define ICM42688_INT_STATUS3               0x38
#define ICM42688_SIGNAL_PATH_RESET         0x4B
#define ICM42688_INTF_CONFIG0              0x4C
#define ICM42688_INTF_CONFIG1              0x4D
#define ICM42688_PWR_MGMT0                 0x4E
#define ICM42688_GYRO_CONFIG0              0x4F
#define ICM42688_ACCEL_CONFIG0             0x50
#define ICM42688_GYRO_CONFIG1              0x51
#define ICM42688_GYRO_ACCEL_CONFIG0        0x52
#define ICM42688_ACCEL_CONFIG1             0x53
#define ICM42688_TMST_CONFIG               0x54
#define ICM42688_APEX_CONFIG0              0x56
#define ICM42688_SMD_CONFIG                0x57
#define ICM42688_FIFO_CONFIG1              0x5F
#define ICM42688_FIFO_CONFIG2              0x60
#define ICM42688_FIFO_CONFIG3              0x61
#define ICM42688_FSYNC_CONFIG              0x62
#define ICM42688_INT_CONFIG0               0x63
#define ICM42688_INT_CONFIG1               0x64
#define ICM42688_INT_SOURCE0               0x65
#define ICM42688_INT_SOURCE1               0x66
#define ICM42688_INT_SOURCE3               0x68
#define ICM42688_INT_SOURCE4               0x69
#define ICM42688_FIFO_LOST_PKT0            0x6C
#define ICM42688_FIFO_LOST_PKT1            0x6D
#define ICM42688_SELF_TEST_CONFIG          0x70
#define ICM42688_WHO_AM_I                  0x75
#define ICM42688_REG_BANK_SEL              0x76 // Banks
#define ICM42688_SENSOR_CONFIG0            0x03
#define ICM42688_GYRO_CONFIG_STATIC2       0x0B
#define ICM42688_GYRO_CONFIG_STATIC3       0x0C
#define ICM42688_GYRO_CONFIG_STATIC4       0x0D
#define ICM42688_GYRO_CONFIG_STATIC5       0x0E
#define ICM42688_GYRO_CONFIG_STATIC6       0x0F
#define ICM42688_GYRO_CONFIG_STATIC7       0x10
#define ICM42688_GYRO_CONFIG_STATIC8       0x11
#define ICM42688_GYRO_CONFIG_STATIC9       0x12
#define ICM42688_GYRO_CONFIG_STATIC10      0x13
#define ICM42688_XG_ST_DATA                0x5F
#define ICM42688_YG_ST_DATA                0x60
#define ICM42688_ZG_ST_DATA                0x61
#define ICM42688_TMSTVAL0                  0x62
#define ICM42688_TMSTVAL1                  0x63
#define ICM42688_TMSTVAL2                  0x64
#define ICM42688_INTF_CONFIG4              0x7A
#define ICM42688_INTF_CONFIG5              0x7B
#define ICM42688_INTF_CONFIG6              0x7C

/**CONTROL DESCRIPTIONS**/
#define DEVICE_RESET 0X01 //DEVICE_CONFIG

enum icm42688_afs
{
	ICM42688_AFS_16G,// default
	ICM42688_AFS_8G,
	ICM42688_AFS_4G,
	ICM42688_AFS_2G,
	NUM_ICM42688__AFS
};
enum icm42688_aodr
{
	ICM42688_AODR_32000HZ,
	ICM42688_AODR_16000HZ,
	ICM42688_AODR_8000HZ,
	ICM42688_AODR_4000HZ,
	ICM42688_AODR_2000HZ,
	ICM42688_AODR_1000HZ,// default
	ICM42688_AODR_200HZ,
	ICM42688_AODR_100HZ,
	ICM42688_AODR_50HZ,
	ICM42688_AODR_25HZ,
	ICM42688_AODR_12_5HZ,
	ICM42688_AODR_6_25HZ,
	ICM42688_AODR_3_125HZ,
	ICM42688_AODR_1_5625HZ,
	ICM42688_AODR_500HZ,
	NUM_ICM42688_AODR
};

enum icm42688_gfs
{
	ICM42688_GFS_2000DPS,// default
	ICM42688_GFS_1000DPS,
	ICM42688_GFS_500DPS,
	ICM42688_GFS_250DPS,
	ICM42688_GFS_125DPS,
	ICM42688_GFS_62_5DPS,
	ICM42688_GFS_31_25DPS,
	ICM42688_GFS_15_625DPS,
	NUM_ICM42688_GFS
};
enum icm42688_godr
{
	ICM42688_GODR_32000HZ,
	ICM42688_GODR_16000HZ,
	ICM42688_GODR_8000HZ,
	ICM42688_GODR_4000HZ,
	ICM42688_GODR_2000HZ,
	ICM42688_GODR_1000HZ,// default
	ICM42688_GODR_200HZ,
	ICM42688_GODR_100HZ,
	ICM42688_GODR_50HZ,
	ICM42688_GODR_25HZ,
	ICM42688_GODR_12_5HZ,
	ICM42688_GODR_X0HZ,
	ICM42688_GODR_X1HZ,
	ICM42688_GODR_X2HZ,
	ICM42688_GODR_500HZ,
	NUM_ICM42688_GODR
};

typedef struct
{
	double gyro[3];
	double acc[3];
	double temp;
	double angle[3];
	uint8_t whoimi;
	_Bool is_init;
}IMU_t;

extern IMU_t IMU;


void Get_ICM42688_Data(void);
void icm42688_init(void);
void Set_LowpassFilter_Range_ICM42688_SPI(enum icm42688_afs afs, enum icm42688_aodr aodr, enum icm42688_gfs gfs, enum icm42688_godr godr);
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num);
// SPI GPIO初始化
void MX_SPI1_GPIO_Init(void);

// SPI数据传输与接收
void SPI1_TransmitReceive(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t length);
void SPI1_Transmit(uint8_t *txBuffer, uint16_t length, uint32_t timeout);
void SPI1_Receive(uint8_t *rxBuffer, uint16_t length, uint32_t timeout);

// ICM42688寄存器读写
static void Write_Data_ICM42688(uint8_t reg, uint8_t dat);
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num);


#endif // __ICM42688_SPI_H