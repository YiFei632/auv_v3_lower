#ifndef __TLV493D_H__
#define __TLV493D_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sim_I2C.h"




typedef struct
{
	int16_t data[4];
	float B[3];//XYZ 单位mT，最大量程130mT（200mT），手机背面10mT左右，磁铁表面的磁场直接报表了满了200以上   1mT=10G 所以量程2000高斯
	float temperature;
	uint8_t error_code;
	_Bool is_ready;
	_Bool is_init;
} TLV493D_t;

enum
{
	TLV493D_No_Error,
	TLV493D_Ack_Error,
	TLV493D_Response_Error
};

extern TLV493D_t TLV493D;;

void tlv493d_init(void);
void Get_TLV493D_Data(void);


#ifdef __cplusplus
}
#endif
#endif /*__WF5803_H__ */