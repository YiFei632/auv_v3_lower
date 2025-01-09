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
	float kp;			 //����ϵ��
	float ki;			 //����ϵ��
	float kd_ex;		 	 //΢��ϵ��
	float kd_fb; //previous_d ΢������
	float k_ff;		 //ǰ�� 
	
	float inte_d_lim;//��������޷�
	float inte_lim;		//integration limit�������޷�		
	float out_lim;  //����޷�
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
float PID_calculate( float dT_s,            //���ڣ���λ���룩
										float in_ff,				//ǰ��ֵ
										float expect,				//����ֵ���趨ֵ��
										float feedback,			//����ֵ����
										PID_t *pid //PID�ṹ��						
										 );	

extern PID_t PID[4];

#ifdef __cplusplus
}
#endif
#endif /*__PID_H__ */