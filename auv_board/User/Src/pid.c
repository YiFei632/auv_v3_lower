#include "pid.h"

_PID_arg_st arg_1[VEC_RPY] ;
_PID_val_st val_1[VEC_RPY] ;

void PID_Init(void)
{
	arg_1[0].kp = 10.f;
	arg_1[0].ki = 0.0f;
	arg_1[0].kd_ex = 0.02f;
	arg_1[0].kd_fb = 0.0f;
	arg_1[0].k_ff = 0.0f;
	
	arg_1[1].kp = 0.02f;
	arg_1[1].ki = 0.01f;
	arg_1[1].kd_ex = 0.0f;
	arg_1[1].kd_fb = 0.0f;
	arg_1[1].k_ff = 0.0f;
}

float PID_calculate( float dT_s,            //周期（单位：秒）
										float in_ff,				//前馈值
										float expect,				//期望值（设定值）
										float feedback,			//反馈值（）
										_PID_arg_st *pid_arg, //PID参数结构体
										_PID_val_st *pid_val,	//PID数据结构体
										float inte_d_lim,//积分误差限幅
										float inte_lim			//integration limit，积分限幅									
										 )	
{
	float differential,hz;
	hz = safe_div(1.0f,dT_s,0);

	
	pid_val->exp_d = (expect - pid_val->exp_old) *hz;
	
	if(pid_arg->fb_d_mode == 0)
	{
		pid_val->fb_d = (feedback - pid_val->feedback_old) *hz;
	}
	else
	{
		pid_val->fb_d = pid_val->fb_d_ex;
	}	
	differential = (pid_arg->kd_ex *pid_val->exp_d - pid_arg->kd_fb *pid_val->fb_d);
	
	pid_val->err = (expect - feedback);	

	pid_val->err_i += pid_arg->ki *LIMIT((pid_val->err ),-inte_d_lim,inte_d_lim )*dT_s;
	pid_val->err_i = LIMIT(pid_val->err_i,-inte_lim,inte_lim);
	
	
	
	pid_val->out = pid_arg->k_ff *in_ff 
	    + pid_arg->kp *pid_val->err  
			+	differential
    	+ pid_val->err_i;
	
	pid_val->feedback_old = feedback;
	pid_val->exp_old = expect;
	
	pid_val->out = LIMIT((pid_val->out), -600, 600);
	
	return (pid_val->out);
}