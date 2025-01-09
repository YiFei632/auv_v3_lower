#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"

#define LED_NUM 10000

void LEDSetRGB(uint8_t R, uint8_t G, uint8_t B);


#ifdef __cplusplus
}
#endif
#endif /*__LED_H__ */