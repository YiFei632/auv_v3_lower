#include "cpg.h"

hopf_cpg_t hopf_cpg = 
{
	.omega = 3,
	.derta_t = 0.01,
	.offset = 0,
	.A2 = 1,
	.scale = 0.35,
	.zero_point = 0,
	.xcur = 0.1,
	.ycur = 0.1
};

void CPG_Init(float zero_posi, float t)
{
	hopf_cpg.zero_point = zero_posi;
	hopf_cpg.derta_t = t;
}

void hopf_cpg_reset()
{
	hopf_cpg.xcur = 0.1;
	hopf_cpg.ycur = 0.1;
}

float hopf_cpg_cal()
{
	float temp_data = hopf_cpg.A2 - hopf_cpg.xcur*hopf_cpg.xcur - (hopf_cpg.ycur - hopf_cpg.offset)*(hopf_cpg.ycur - hopf_cpg.offset);
	float xdiff = -hopf_cpg.omega*(hopf_cpg.ycur - hopf_cpg.offset) + hopf_cpg.xcur*temp_data;
	float ydiff = hopf_cpg.omega*hopf_cpg.xcur + (hopf_cpg.ycur - hopf_cpg.offset)*temp_data;
	hopf_cpg.xcur = hopf_cpg.xcur + hopf_cpg.derta_t * xdiff;
	hopf_cpg.ycur = hopf_cpg.ycur + hopf_cpg.derta_t * ydiff;
	
	return hopf_cpg.ycur*hopf_cpg.scale;
}

//float d1 = 15, d2 = 10;
//float get_motor_angle(float fish_tail_angle)
//{
//	return (float)225/16*arcSinByLut(d1/d2*SinByLut(fish_tail_angle))+hopf_cpg.zero_point;
//}