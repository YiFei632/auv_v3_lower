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
#include "control.h" 


void lora_data(uint8_t data);
void lora_data_full(uint8_t * data);
void lora_data_test(uint8_t data);
void RS485_data(uint8_t data);
void motor_control(uint8_t motor_num, uint8_t mode, int16_t data);
void motor_control_cpg(uint8_t motor_num);
void receive_conv(uint8_t data);

#ifdef __cplusplus
}
#endif
#endif /*__DATA_H__ */