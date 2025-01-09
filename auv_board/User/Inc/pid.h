#ifndef __PID_H__
#define __PID_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )


enum
{
	ROL = 0,
	PIT = 1,
	YAW = 2,
	VEC_RPY,
};

enum
{
	KP = 0,
	KI = 1,
	KD = 2,
	PID,
};


typedef struct
{
	uint8_t fb_d_mode;
	float kp;			 //����ϵ��
	float ki;			 //����ϵ��
	float kd_ex;		 	 //΢��ϵ��
	float kd_fb; //previous_d ΢������
	float k_ff;		 //ǰ�� 

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


void PID_Init(void);
float PID_calculate( float T,            //����
										float in_ff,				//ǰ��
										float expect,				//����ֵ���趨ֵ��
										float feedback,			//����ֵ
										_PID_arg_st *pid_arg, //PID�����ṹ��
										_PID_val_st *pid_val,	//PID���ݽṹ��
										float inte_d_lim,
										float inte_lim			//integration limit�������޷�
										   );			//���

										
extern _PID_arg_st arg_1[VEC_RPY] ;
extern _PID_val_st val_1[VEC_RPY] ;

#ifdef __cplusplus
}
#endif
#endif /*__PID_H__ */