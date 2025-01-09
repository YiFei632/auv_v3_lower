#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )

typedef struct
{
	uint8_t fb_d_mode;
	float kp;			 //比例系数
	float ki;			 //积分系数
	float kd_ex;		 	 //微分系数
	float kd_fb; //previous_d 微分先行
	float k_ff;		 //前馈 
	
	float inte_d_lim;//积分误差限幅
	float inte_lim;		//integration limit，积分限幅		
	float out_lim;  //输出限辐
}_PID_arg_st;

typedef struct
{
	float err;
	float exp_old;
	float feedback_old;
	
	float fb_d;
	float fb_d_ex;
	float exp_d;
//	float err_d_lpf;
	float err_i;
	float ff;
	float pre_d;

	float out;
}_PID_val_st;

typedef struct
{
	_PID_arg_st arg;
	_PID_val_st val;
}PID_t;


void PID_Init(void);
float PID_calculate( float dT_s,            //周期（单位：秒）
										float in_ff,				//前馈值
										float expect,				//期望值（设定值）
										float feedback,			//反馈值（）
										PID_t *pid //PID结构体						
										 );	

extern PID_t PID[4];

#ifdef __cplusplus
}
#endif
#endif /*__PID_H__ */