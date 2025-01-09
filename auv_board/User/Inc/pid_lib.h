/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
 #ifndef __PID_LIB_H
#define __PID_LIB_H
#ifdef __cplusplus
extern "C" {
#endif 

#include <math.h>
	
#define SPEED_SUM 50
#define ACCEL_MAX 10




//????: ????
typedef struct 
{
	double alpha_set[3]; //???????

	double omega_set[3]; //??????,
	double accel_set[3]; //????????(????,kp?ki?kd)
	double speed_set[3]; //???????
}pid_outputHandler;

typedef struct
{
	double angle_err[3]; //??????,????
	double omega_err[3]; //???????,????
	double speed_err[3]; //???????
	double pos_err[3]; //??????
}pid_errHandler;

typedef struct
{
	double alpha_pid[3][3]; //?????pid,???????????roll?pitch?yaw,???????????kp?ki?kd,??
	double omega_pid[3][3]; //????pid
	double accel_pid[3]; //??????pid(????,kp?ki?kd)
	double pos_pid[3]; //?????pid(????,kp?ki?kd)(??Z??????)
}pid_parameterHandler;



typedef struct
{
	double speed_sum_max;
	double angle_sum_max[3];
	double omega_sum_max[3];//????

	double accel_output_max;
	double omega_output_max[3];
	double alpha_output_max[3];//????
}pid_ParamTypedef;//(?????,??).

typedef struct
{
	pid_errHandler pid_err;

	pid_errHandler pid_err_last;
	pid_errHandler pid_derr;//???????

	pid_errHandler pid_sum;//???????

	pid_parameterHandler pid_param;

	pid_outputHandler pid_output;

	pid_ParamTypedef pid_max;
}pid_HandlerTypedef;

void pparam_init(pid_ParamTypedef* pparam, double speed_sum, double angle_sum[], double omega_sum[], double accel_output, double omega_output[], double alpha_output[]);
void pid_init(pid_HandlerTypedef* pid);
void pid_setfrontbackparam(pid_HandlerTypedef* pid);
void pid_setupdownparam(pid_HandlerTypedef* pid);

void pid_calc(pid_HandlerTypedef* pid, double angle_fdb[], double omega_fdb[], double speed_tgt[], double speed_fdb[]);
void pid_with_pos_calc(pid_HandlerTypedef* pid, double angle_fdb[], double omega_fdb[], double pos_tgt, double pos_fdb, double speed_fdb[]);


#endif
#ifdef __cplusplus
}
#endif
