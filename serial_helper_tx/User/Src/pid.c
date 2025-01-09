#include "pid.h"

PID_t PID[4];

void PID_Init(void)
{
	PID[0].arg.kp = 0.02f;
	PID[0].arg.ki = 0.0f;
	PID[0].arg.kd_ex = 0.0f;
	PID[0].arg.kd_fb = 0.0f;
	PID[0].arg.k_ff = 0.0f;
	PID[0].arg.inte_d_lim = 500;
	PID[0].arg.inte_lim = 500;
	PID[0].arg.out_lim = 3000;
	
	PID[1].arg.kp = 0.01f;
	PID[1].arg.ki = 0.0f;
	PID[1].arg.kd_ex = 0.0f;
	PID[1].arg.kd_fb = 0.0f;
	PID[1].arg.k_ff = 0.0f;
	PID[1].arg.inte_d_lim = 700;
	PID[1].arg.inte_lim = 500;
	PID[1].arg.out_lim = 3000;
	
	PID[2].arg.kp = 0.35f;
	PID[2].arg.ki = 0.0f;
	PID[2].arg.kd_ex = 0.0f;
	PID[2].arg.kd_fb = 0.0f;
	PID[2].arg.k_ff = 0.0f;
	PID[2].arg.inte_d_lim = 30;
	PID[2].arg.inte_lim = 1;
	PID[2].arg.out_lim = 3000;
	
	PID[3].arg.kp = 20.0f;
	PID[3].arg.ki = 0.0f;
	PID[3].arg.kd_ex = 0.1f;
	PID[3].arg.kd_fb = 0.0f;
	PID[3].arg.k_ff = 0.0f;
	PID[3].arg.inte_d_lim = 10;
	PID[3].arg.inte_lim = 5;
	PID[3].arg.out_lim = 800;
}

float PID_calculate( float dT_s,            //周期（单位：秒）
										float in_ff,				//前馈值
										float expect,				//期望值（设定值）
										float feedback,			//反馈值（）
										PID_t *pid //PID结构体						
										 )	
{
	float differential,hz;
	hz = safe_div(1.0f,dT_s,0);

	
	pid->val.exp_d = (expect - pid->val.exp_old) *hz;
	
	if(pid->arg.fb_d_mode == 0)
	{
		pid->val.fb_d = (feedback - pid->val.feedback_old) *hz;
	}
	else
	{
		pid->val.fb_d = pid->val.fb_d_ex;
	}	
	differential = (pid->arg.kd_ex *pid->val.exp_d - pid->arg.kd_fb *pid->val.fb_d);
	
	pid->val.err = (expect - feedback);	

	pid->val.err_i += pid->arg.ki *LIMIT((pid->val.err ),-pid->arg.inte_d_lim,pid->arg.inte_d_lim )*dT_s;
	pid->val.err_i = LIMIT(pid->val.err_i,-pid->arg.inte_lim,pid->arg.inte_lim);
	
	
	pid->val.out = pid->arg.k_ff *in_ff 
	    + pid->arg.kp *pid->val.err  
			+	differential
    	+ pid->val.err_i;
	
	pid->val.feedback_old = feedback;
	pid->val.exp_old = expect;
	
	pid->val.out = LIMIT((pid->val.out), -pid->arg.out_lim, pid->arg.out_lim);
	
	return (pid->val.out);
}