#ifndef __KERNAL_H__
#define __KERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "telecontrol.h"
#include "control.h"
#include "icm_42688.h"

void stop_all_IT(void);

#ifdef __cplusplus
}
#endif
#endif /*__KERNAL_H__ */