#ifndef __CPG_H__
#define __CPG_H__

#include "main.h"

typedef struct 
{
	float omega;
	float derta_t;
	float offset;
	float A2;
	
	float scale;
	float zero_point;
	
	float xcur, ycur;
}hopf_cpg_t;

void CPG_Init(float zero_posi, float t);
void hopf_cpg_reset(void);
float hopf_cpg_cal(void);
//float get_motor_angle(float fish_tail_angle);

extern hopf_cpg_t hopf_cpg;

#endif

