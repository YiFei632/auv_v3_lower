#ifndef __DATA_H__
#define __DATA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lora.h"
#include <string.h>
#include "telecontrol.h"

typedef enum
{
	velocity_mode,
	position_mode,
}motor_mode_t;

void lora_data(uint8_t data);
void lora_data_full(uint8_t * data);
void lora_data_test(uint8_t data);
void receive_conv(uint8_t data);


#ifdef __cplusplus
}
#endif
#endif /*__DATA_H__ */