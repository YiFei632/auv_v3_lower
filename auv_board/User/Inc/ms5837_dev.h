#ifndef __MS5837_DEV_H__
#define __MS5837_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sim_I2C.h"

	
/* Defines ------------------------------------------------------------------*/


#define MS5837_ADDR      0xEC//地址区

#define CMD_RST          0x1E//命令区
#define CMD_ADC_READ     0x00
#define CMD_PROM_READ    0xA0
#define CMD_CONVERT_D1   0x40
#define CMD_CONVERT_D2   0x50
	
#define CONVERT_PA       100.0f//校准区
#define CONVERT_BAR      0.001f
#define CONVERT_MBAR     1.0f

//#define OSR_8192         0x05



	
/* Typedef ------------------------------------------------------------------*/
typedef enum {
	MS5837_30BA = 0x01,
	MS5837_02BA = 0x00,
}ms5837_type_e;

typedef enum {
	freshwater = 997,
	seawater = 1029,
}ms5837_density_e;

typedef struct{
	ms5837_type_e model;
	ms5837_density_e density;
	uint8_t addr;
	
	float pressure;
	float temperature;
	//float altitude;
	float depth;
}ms5837_HandleTypedef;

/* Variables ------------------------------------------------------------------*/
extern ms5837_HandleTypedef sensor;
extern sim_I2C_t i2c2;
/* Functions ------------------------------------------------------------------*/
uint16_t readFromMs5837(ms5837_HandleTypedef* sensor,uint8_t add);
uint8_t crc4(uint16_t n_prom[]);
uint8_t ms5837_init(ms5837_HandleTypedef* sensor, ms5837_type_e type, ms5837_density_e density);
void ms5837_read(ms5837_HandleTypedef* sensor);
void ms5837_calculate(ms5837_HandleTypedef* sensor);

void ms5837_getpressure(ms5837_HandleTypedef* sensor, float conv);
void ms5837_gettemperature(ms5837_HandleTypedef* sensor);
void ms5837_getdepth(ms5837_HandleTypedef* sensor);

#ifdef __cplusplus
}
#endif
#endif /*__MS5837_DEV_H__ */
