/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       推力解算的相关库文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#include "thrust_lib.h"

float32_t  ix[4] = {2.374,0,0,2.705};
float32_t  iy[4] = {0.448,0,0,40};
float32_t  dx[4] = {0.65,0,0,0.65};
float32_t  dy[4] = {0.55,0,0,1};

float32_t fx_fdb[2] = {0,0};
float32_t fy_fdb[2] = {0,0};

float32_t ctrl_data[4] = {0,0,0,0};

arm_matrix_instance_f32 IX;
arm_matrix_instance_f32 IY;
arm_matrix_instance_f32 DX;
arm_matrix_instance_f32 DY;

arm_matrix_instance_f32 FX_FDB;
arm_matrix_instance_f32 FY_FDB;

void control_data_init(void){//初始化控制量
	arm_mat_init_f32(&IX,2,2,ix);
	arm_mat_init_f32(&IY,2,2,iy);
	arm_mat_init_f32(&DX,2,2,dx);
	arm_mat_init_f32(&DY,2,2,dy);
}

arm_status sta_x;
void control_data_calc_x(double alpha_roll_set, double alpha_pitch_set, double alpha_yaw_set,double accel_set){
	fx_fdb[0] = alpha_yaw_set;
	fx_fdb[1] = alpha_pitch_set;
	fy_fdb[0] = alpha_roll_set;
	fy_fdb[1] = accel_set;
	
	arm_mat_init_f32(&FX_FDB,(sizeof(fx_fdb) / sizeof(fx_fdb[0])),(sizeof(fx_fdb[0]) / sizeof(fx_fdb[0])),fx_fdb);
	arm_mat_init_f32(&FY_FDB,(sizeof(fy_fdb) / sizeof(fy_fdb[0])),(sizeof(fy_fdb[0]) / sizeof(fy_fdb[0])),fy_fdb);
	//求逆
	float32_t dx_inv[4];
	float32_t dy_inv[4];
	
	arm_matrix_instance_f32 DX_INV=
	{
		.numCols = 2,
		.numRows = 2,
		.pData = dx_inv
	};
	
	arm_matrix_instance_f32 DY_INV=
	{
		.numCols = 2,
		.numRows = 2,
		.pData = dy_inv
	};
	
	sta_x = arm_mat_inverse_f32(&DX,&DX_INV);
	sta_x = arm_mat_inverse_f32(&DY,&DY_INV);
	//相乘
	float32_t fx_mul[2];
	float32_t fy_mul[2];
	
	float32_t fx[2];
	float32_t fy[2];
	
	
	arm_matrix_instance_f32 FX_MUL={
		.numRows = (sizeof(fx_mul) / sizeof(fx_mul[0])),
		.numCols = (sizeof(fx_mul[0]) / sizeof(fx_mul[0])),
		.pData = fx_mul
	};
	arm_matrix_instance_f32 FY_MUL={
		.numRows = (sizeof(fy_mul) / sizeof(fy_mul[0])),
		.numCols = (sizeof(fy_mul[0]) / sizeof(fy_mul[0])),
		.pData = fy_mul
	};
	
	arm_matrix_instance_f32 FX={
		.numRows = (sizeof(fx) / sizeof(fx[0])),
		.numCols = (sizeof(fx[0]) / sizeof(fx[0])),
		.pData = fx
	};
	arm_matrix_instance_f32 FY={
		.numRows = (sizeof(fy) / sizeof(fy[0])),
		.numCols = (sizeof(fy[0]) / sizeof(fy[0])),
		.pData = fy
	};
	
	
	arm_mat_mult_f32(&IX, &DX_INV, &FX_MUL);
	arm_mat_mult_f32(&IY, &DY_INV, &FY_MUL);
	
	arm_mat_mult_f32(&FX_MUL, &FX_FDB, &FX);
	arm_mat_mult_f32(&FY_MUL, &FY_FDB, &FY);
	//开始提取量进行计算
	ctrl_data[0] = sqrtf(powf(FX.pData[0],2)+powf(FX.pData[1],2));
	ctrl_data[1] = sqrtf(powf(FY.pData[0],2)+powf(FY.pData[1],2));
	ctrl_data[2] = atanf(FX.pData[0] / FX.pData[1]);
	ctrl_data[3] = atanf(FY.pData[0] / FY.pData[1]);
	
	if (accel_set < 0) {
		ctrl_data[1] = -ctrl_data[1];
		ctrl_data[3] = -ctrl_data[3];
	}

	if (alpha_pitch_set < 0) {
		ctrl_data[0] = -ctrl_data[0];
		ctrl_data[2] = -ctrl_data[2];
	}
	
}

arm_status sta_z;
void control_data_calc_z(double alpha_roll_set, double alpha_pitch_set, double alpha_yaw_set,double accel_set){
	fx_fdb[0] = alpha_yaw_set;
	fx_fdb[1] = alpha_pitch_set;
	fy_fdb[0] = alpha_roll_set;
	fy_fdb[1] = accel_set;
	
	arm_mat_init_f32(&FX_FDB,(sizeof(fx_fdb) / sizeof(fx_fdb[0])),(sizeof(fx_fdb[0]) / sizeof(fx_fdb[0])),fx_fdb);
	arm_mat_init_f32(&FY_FDB,(sizeof(fy_fdb) / sizeof(fy_fdb[0])),(sizeof(fy_fdb[0]) / sizeof(fy_fdb[0])),fy_fdb);
	//求逆
	float32_t dx_inv[4];
	float32_t dy_inv[4];
	
	arm_matrix_instance_f32 DX_INV=
	{
		.numCols = 2,
		.numRows = 2,
		.pData = dx_inv
	};
	
	arm_matrix_instance_f32 DY_INV=
	{
		.numCols = 2,
		.numRows = 2,
		.pData = dy_inv
	};
	
	sta_z = arm_mat_inverse_f32(&DX,&DX_INV);
	sta_z = arm_mat_inverse_f32(&DY,&DY_INV);
	//相乘
	float32_t fx_mul[2];
	float32_t fy_mul[2];
	
	float32_t fx[2];
	float32_t fy[2];
	
	
	arm_matrix_instance_f32 FX_MUL={
		.numRows = (sizeof(fx_mul) / sizeof(fx_mul[0])),
		.numCols = (sizeof(fx_mul[0]) / sizeof(fx_mul[0])),
		.pData = fx_mul
	};
	arm_matrix_instance_f32 FY_MUL={
		.numRows = (sizeof(fy_mul) / sizeof(fy_mul[0])),
		.numCols = (sizeof(fy_mul[0]) / sizeof(fy_mul[0])),
		.pData = fy_mul
	};
	
	arm_matrix_instance_f32 FX={
		.numRows = (sizeof(fx) / sizeof(fx[0])),
		.numCols = (sizeof(fx[0]) / sizeof(fx[0])),
		.pData = fx
	};
	arm_matrix_instance_f32 FY={
		.numRows = (sizeof(fy) / sizeof(fy[0])),
		.numCols = (sizeof(fy[0]) / sizeof(fy[0])),
		.pData = fy
	};
	
	
	sta_z = arm_mat_mult_f32(&IX, &DX_INV, &FX_MUL);
	sta_z = arm_mat_mult_f32(&IY, &DY_INV, &FY_MUL);
	
	sta_z = arm_mat_mult_f32(&FX_MUL, &FX_FDB, &FX);
	sta_z = arm_mat_mult_f32(&FY_MUL, &FY_FDB, &FY);
	//开始提取量进行计算
	ctrl_data[0] = sqrtf(powf(FX.pData[0],2)+powf(FX.pData[1],2));
	ctrl_data[1] = atanf(FX.pData[0] / FX.pData[1]);
	ctrl_data[2] = (FY.pData[1] + FY.pData[0]) / 2;
	ctrl_data[3] = (FY.pData[1] - FY.pData[0]) / 2;
	
//	if (accel_set < 0) {
//		ctrl_data[1] = -ctrl_data[1];
//		ctrl_data[3] = -ctrl_data[3];
//	}

	if (alpha_pitch_set < 0) {
		ctrl_data[0] = -ctrl_data[0];
		ctrl_data[1] = -ctrl_data[1];
	}
	
}
