#ifndef __WF5803_H__
#define __WF5803_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sim_I2C.h"

#define WF5803_DATA_ADDR 0x06
#define WF5803_CMD_ADDR 0x30
#define WF5803_CMD_CONTINEOUS 0x0B
#define WF5803_CMD_SINGLE 0x0A
#define WF5803_STATUS_ADDR 0x02

#define PRESSURE_MEASURE_COUNT 20  // 定义测量次数，用于数据采集


typedef struct
{
	double pressure;
	double temperature;
	double air_pressure;
	double depth;
	uint8_t error_code;
	_Bool is_ready;
	_Bool is_init;
} WF5803_t;

extern WF5803_t WF5803;

void wf5803_init(void);
void wf5803_getdata(void);

#ifdef __cplusplus
}
#endif
#endif /*__WF5803_H__ */