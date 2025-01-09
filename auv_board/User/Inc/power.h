#ifndef __POWER_H__
#define __POWER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"

typedef struct
{
	uint16_t raw_data[2];
	double k, b;
	double voltage;
	double current;
}power_t;

extern power_t power;

void get_power_data(void);

#ifdef __cplusplus
}
#endif
#endif /*__POWER_H__ */