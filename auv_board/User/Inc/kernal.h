#ifndef __KERNAL_H__
#define __KERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "wf5803.h"
//#include "icm_42688.h"
#include "control.h"
#include "SDCard.h"
#include "lora.h"
#include "data.h"
#include "sim_SPItwo.h"

//#include "ms5837_dev.h"
#include "remote_task.h"
#include "sim_serial.h"

#define record_num 500

typedef struct
{
	uint32_t start_time, last_start_time, end_time, dur_time, timer_dur;
}time_manage_t;

extern _Bool is_record;
extern _Bool is_storage;
extern _Bool has_storaged;
extern _Bool is_record_flash;

//extern _Bool want_read;
//extern _Bool read_info;
//extern _Bool read_data;
extern IMU_t imu_record[record_num];
//extern WF5803_t wf5803_record[record_num];

void stop_all_IT(void);
void start_all_IT(void);

#ifdef __cplusplus
}
#endif
#endif /*__KERNAL_H__ */