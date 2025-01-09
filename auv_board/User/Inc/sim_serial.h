#ifndef __SIM_SERIAL_H__
#define __SIM_SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sim_I2C.h"
#include <stdio.h>  // ?? vsnprintf ??
#include <stdarg.h>  // ???????

typedef struct
{
	User_PIN_t TX;
	User_PIN_t RX;
	uint8_t delay_time;
}sim_serial_t;

extern sim_serial_t serial1;

void serial_send_byte(sim_serial_t * serial, uint8_t data);
void serial_send_str(sim_serial_t * serial, char *str);
void serial_send_formatted(sim_serial_t *serial, const char *format, ...);
void serial_send_data(sim_serial_t *serial, uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif /*__SIM_SERIAL_H__ */