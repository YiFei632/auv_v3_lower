#ifndef __TELECONTROL_H__
#define __TELECONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef struct 
{
	_Bool key_left_rocker, key_right_rocker;
	_Bool up, down, left, right;
	_Bool X, Y, A, B;

}key_state_struct_t;

typedef struct 
{
	key_state_struct_t key_state;
	key_state_struct_t handled_key_state;
	_Bool dial[4];
	uint16_t rocker[6];
	_Bool is_receive;
}telecontrol_struct_t;


#define lora_LENGTH 17
#define lora_LENGTH_short 10
#define short_rocker_max 255
#define long_rocker_max 65535

extern telecontrol_struct_t telecontrol;


#ifdef __cplusplus
}
#endif
#endif /*__TELECONTROL_H__ */