/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#ifndef __REMOTE_TASK_H
#define __REMOTE_TASK_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "lora.h"
#include "data.h"
#include "telecontrol.h"
#include "motor_dev.h"
	
//#define PI 3.1415926

#define MAX_ERR    150.0f
#define MIN_ERR    4.0f
#define MIDDLE_ERR 80.0f

typedef enum{
	FORWARD_BACK = 0,
	LEFT_RIGHT = 1,
	UP_DOWN = 2,
	NO_MOOD = 3
}AUV_STATUS;

typedef struct{
	float speed_ref_x;
	float speed_ref_y;
	
	float angle_ref_x;
	float angle_ref_y;
	
	AUV_STATUS status;
}AUV_RemoteHandleTypedef;

typedef struct{
	float speed_ref_x;
	float speed_ref_yleft;
	float speed_ref_yright;
	
	float angle_ref_x;
	float angle_ref_y;
	
}AUV_HeightHandleTypedef;

extern AUV_RemoteHandleTypedef auv_remote;
extern AUV_HeightHandleTypedef auv_height;


void auv_getstatus(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol);
void auv_setspeedref(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol);
void auv_setangleref(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol);
void auv_setspeedref_height(AUV_HeightHandleTypedef* height, float speed_x, float speed_yl, float speed_yr);
void auv_setangleref_height(AUV_HeightHandleTypedef* height,float angle_x);
void auv_control();
void auv_control_height();
	
#endif
#ifdef __cplusplus
}
#endif
