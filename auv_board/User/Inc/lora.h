#ifndef __LORA_H__
#define __LORA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "data.h"
#include "telecontrol.h"

#define RX_BUFFER_SIZE 32

typedef enum
{
	short_data,
	long_full_data
}lora_data_type;

typedef struct
{
	lora_data_type type;
	_Bool is_init;
}E70_433MT14S_t;

typedef enum{
	SD_READ_INFO = 0,
	SD_INFO_PLUS = 1,
	SD_INFO_MINUS = 2,
	SD_READ_DATA = 3,
	RECORD_DATA = 4,
	SENT_DATA = 5,
	SD_NO_COMMAND = 6
}sdcard_command;

extern sdcard_command sd_cmd;

extern uint8_t RxBuffer_1[RX_BUFFER_SIZE], RxBuffer_2[RX_BUFFER_SIZE], RxBuffer_3[RX_BUFFER_SIZE];   //接收数据
//extern uint8_t RxBuffer_CMD[RX_BUFFER_SIZE];
extern uint8_t aRxBuffer_1, aRxBuffer_2, aRxBuffer_3;//HAL库使用的串口接收缓冲
extern E70_433MT14S_t E70_433MT14S;

void lora_mode(uint8_t M0, uint8_t M1, uint8_t M2);
void lora_mode_contact(void);
void lora_mode_config(void);
void lora_set_baudrate(void);
void E70_433MT14S_init(void);


#ifdef __cplusplus
}
#endif
#endif /*__LORA_H__ */