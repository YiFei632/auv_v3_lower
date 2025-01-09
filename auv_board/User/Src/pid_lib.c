/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       PID的相关库文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#include "pid_lib.h"

pid_HandlerTypedef auv_pid;
//pid_ParamTypedef auv_pparam;

void pparam_init(pid_ParamTypedef* pparam, double speed_sum, double angle_sum[], double omega_sum[], double accel_output, double omega_output[], double alpha_output[]) {
	pparam->speed_sum_max = speed_sum;
	pparam->accel_output_max = accel_output;
	for (int i = 0; i < 3; i++) {
		pparam->angle_sum_max[i] = angle_sum[i];
		pparam->omega_sum_max[i] = omega_sum[i];

		pparam->omega_output_max[i] = omega_output[i];
		pparam->alpha_output_max[i] = alpha_output[i];
	}
}

void pid_init(pid_HandlerTypedef* pid) {
	for (int i = 0; i < 3; i++) {
		pid->pid_err.angle_err[i] = 0;
		pid->pid_err.omega_err[i] = 0;
		pid->pid_err.speed_err[i] = 0;

		pid->pid_err_last.angle_err[i] = 0;
		pid->pid_err_last.omega_err[i] = 0;
		pid->pid_err_last.speed_err[i] = 0;

		pid->pid_derr.angle_err[i] = 0;
		pid->pid_derr.omega_err[i] = 0;
		pid->pid_derr.speed_err[i] = 0;

		pid->pid_sum.angle_err[i] = 0;
		pid->pid_sum.omega_err[i] = 0;
		pid->pid_sum.speed_err[i] = 0;

		pid->pid_output.omega_set[i] = 0;
		pid->pid_output.alpha_set[i] = 0;
		pid->pid_output.accel_set[i] = 0;

		for (int j = 0; j < 3; j++) {
			pid->pid_param.alpha_pid[i][j] = 0;
			pid->pid_param.omega_pid[i][j] = 0;
		}
		pid->pid_param.accel_pid[i] = 0;
	}

	//pparam_init(&(auv_pparam),1,[1.5708 1.5708 1.5708],[1.5708 1.5708 1.5708],)
}

void pid_setfrontbackparam(pid_HandlerTypedef* pid) {
	pid->pid_param.accel_pid[0] = 10;
	pid->pid_param.accel_pid[1] = 0.001;
	pid->pid_param.accel_pid[2] = 5;
	
	pid->pid_param.omega_pid[1][0] = 1;
	pid->pid_param.omega_pid[2][0] = 1;

	pid->pid_param.alpha_pid[1][0] = 1;
	pid->pid_param.alpha_pid[2][0] = 1;

	pid->pid_max.speed_sum_max = SPEED_SUM;
	//pid->pid_max.accel_output_max = ACCEL_MAX;
}

void pid_setupdownparam(pid_HandlerTypedef* pid) {
	pid->pid_param.pos_pid[0] = 2;
	pid->pid_param.pos_pid[1] = 0.01;
	pid->pid_param.pos_pid[2] = 0.1;
	
	pid->pid_param.accel_pid[0] = 1;
	pid->pid_param.accel_pid[1] = 0;
	pid->pid_param.accel_pid[2] = 0;

	pid->pid_param.omega_pid[0][0] = 0.5;
	pid->pid_param.omega_pid[1][0] = 0.5;
	pid->pid_param.omega_pid[2][0] = 0.5;

	pid->pid_param.omega_pid[0][1] = 0.01;
	pid->pid_param.omega_pid[1][1] = 0.01;
	pid->pid_param.omega_pid[2][1] = 0.01;

	pid->pid_param.omega_pid[0][2] = 0.1;
	pid->pid_param.omega_pid[1][2] = 0.1;
	pid->pid_param.omega_pid[2][2] = 0.1;


	pid->pid_param.alpha_pid[0][0] = 0.2;
	pid->pid_param.alpha_pid[1][0] = 0.2;
	pid->pid_param.alpha_pid[2][0] = 0.2;

	pid->pid_param.alpha_pid[0][1] = 0.0;
	pid->pid_param.alpha_pid[1][1] = 0.0;
	pid->pid_param.alpha_pid[2][1] = 0.0;

	pid->pid_param.alpha_pid[0][2] = 0;
	pid->pid_param.alpha_pid[1][2] = 0;
	pid->pid_param.alpha_pid[2][2] = 0;

	pid->pid_max.speed_sum_max = SPEED_SUM;
	//pid->pid_max.accel_output_max = ACCEL_MAX;
}

void pid_calc(pid_HandlerTypedef* pid, double angle_fdb[], double omega_fdb[], double speed_tgt[], double speed_fdb[]) {
	for (int i = 0; i < 3; i++) {
		pid->pid_err_last.speed_err[i] = pid->pid_err.speed_err[i];

		pid->pid_err.speed_err[i] = speed_tgt[i] - speed_fdb[i];

		

		//if (&(pid->pid_err.speed_err[i]) != nan && &(pid->pid_sum.speed_err[i]) != nan) {
		pid->pid_sum.speed_err[i] = pid->pid_sum.speed_err[i] + pid->pid_err.speed_err[i];
		//}
		//else {
		//	pid->pid_sum.speed_err[i] = 0;
		//}

		pid->pid_derr.speed_err[i] = pid->pid_err.speed_err[i] - pid->pid_err_last.speed_err[i];
		
		pid->pid_output.accel_set[i] = pid->pid_param.accel_pid[0] * pid->pid_err.speed_err[i] + pid->pid_param.accel_pid[2] * pid->pid_derr.speed_err[i] + pid->pid_param.accel_pid[1] * pid->pid_sum.speed_err[i];
	
	}

	for (int i = 0; i < 3; i++) {
		pid->pid_err_last.angle_err[i] = pid->pid_err.angle_err[i];

		pid->pid_err.angle_err[i] = 0 - angle_fdb[i];

		pid->pid_sum.angle_err[i] = pid->pid_sum.angle_err[i] + pid->pid_err.angle_err[i];

		pid->pid_derr.angle_err[i] = pid->pid_err.angle_err[i] - pid->pid_err_last.angle_err[i];

		pid->pid_output.omega_set[i] = pid->pid_param.omega_pid[i][0] * pid->pid_err.angle_err[i] +pid->pid_param.omega_pid[i][1] * pid->pid_sum.angle_err[i] + pid->pid_param.omega_pid[i][2] * pid->pid_derr.angle_err[i];
	}

	for (int i = 0; i < 3; i++) {
		pid->pid_err_last.omega_err[i] = pid->pid_err.omega_err[i];

		pid->pid_err.omega_err[i] = pid->pid_output.omega_set[i] - omega_fdb[i];

		pid->pid_sum.omega_err[i] = pid->pid_sum.omega_err[i] + pid->pid_err.omega_err[i];

		pid->pid_derr.omega_err[i] = pid->pid_err.omega_err[i] - pid->pid_err_last.omega_err[i];

		pid->pid_output.alpha_set[i] = pid->pid_param.alpha_pid[i][0] * pid->pid_err.omega_err[i] +pid->pid_param.alpha_pid[i][1] * pid->pid_sum.omega_err[i] + pid->pid_param.alpha_pid[i][2] * pid->pid_derr.omega_err[i];
	}
	
}
void pid_with_pos_calc(pid_HandlerTypedef* pid, double angle_fdb[], double omega_fdb[], double pos_tgt, double pos_fdb, double speed_fdb[]) {
	//for (int i = 0; i < 3; i++) {
		pid->pid_err_last.pos_err[2] = pid->pid_err.pos_err[2];

		pid->pid_err.pos_err[2] = pos_tgt - pos_fdb;



		//if (&(pid->pid_err.speed_err[i]) != nan && &(pid->pid_sum.speed_err[i]) != nan) {
		pid->pid_sum.pos_err[2] = pid->pid_sum.pos_err[2] + pid->pid_err.pos_err[2];
		//}
		//else {
		//	pid->pid_sum.speed_err[i] = 0;
		//}

		pid->pid_derr.pos_err[2] = pid->pid_err.pos_err[2] - pid->pid_err_last.pos_err[2];

		pid->pid_output.speed_set[2] = pid->pid_param.pos_pid[0] * pid->pid_err.pos_err[2] + pid->pid_param.pos_pid[2] * pid->pid_derr.pos_err[2] + pid->pid_param.pos_pid[1] * pid->pid_sum.pos_err[2];

		for (int i = 0; i < 3; i++) {
			pid->pid_err_last.speed_err[i] = pid->pid_err.speed_err[i];

			pid->pid_err.speed_err[i] = pid->pid_output.speed_set[i] - speed_fdb[i];



			//if (&(pid->pid_err.speed_err[i]) != nan && &(pid->pid_sum.speed_err[i]) != nan) {
			pid->pid_sum.speed_err[i] = pid->pid_sum.speed_err[i] + pid->pid_err.speed_err[i];
			//}
			//else {
			//	pid->pid_sum.speed_err[i] = 0;
			//}

			pid->pid_derr.speed_err[i] = pid->pid_err.speed_err[i] - pid->pid_err_last.speed_err[i];

			pid->pid_output.accel_set[i] = pid->pid_param.accel_pid[0] * pid->pid_err.speed_err[i] + pid->pid_param.accel_pid[2] * pid->pid_derr.speed_err[i] + pid->pid_param.accel_pid[1] * pid->pid_sum.speed_err[i];
		}
		



	//}

	for (int i = 0; i < 3; i++) {
		pid->pid_err_last.angle_err[i] = pid->pid_err.angle_err[i];

		pid->pid_err.angle_err[i] = 0 - angle_fdb[i];

		pid->pid_sum.angle_err[i] = pid->pid_sum.angle_err[i] + pid->pid_err.angle_err[i];

		pid->pid_derr.angle_err[i] = pid->pid_err.angle_err[i] - pid->pid_err_last.angle_err[i];

		pid->pid_output.omega_set[i] = pid->pid_param.omega_pid[i][0] * pid->pid_err.angle_err[i] + pid->pid_param.omega_pid[i][1] * pid->pid_sum.angle_err[i] + pid->pid_param.omega_pid[i][2] * pid->pid_derr.angle_err[i];
	}

	for (int i = 0; i < 3; i++) {
		pid->pid_err_last.omega_err[i] = pid->pid_err.omega_err[i];

		pid->pid_err.omega_err[i] = pid->pid_output.omega_set[i] - omega_fdb[i];

		pid->pid_sum.omega_err[i] = pid->pid_sum.omega_err[i] + pid->pid_err.omega_err[i];

		pid->pid_derr.omega_err[i] = pid->pid_err.omega_err[i] - pid->pid_err_last.omega_err[i];

		pid->pid_output.alpha_set[i] = pid->pid_param.alpha_pid[i][0] * pid->pid_err.omega_err[i] + pid->pid_param.alpha_pid[i][1] * pid->pid_sum.omega_err[i] + pid->pid_param.alpha_pid[i][2] * pid->pid_derr.omega_err[i];
	}
}
