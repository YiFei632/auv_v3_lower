/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#ifndef __THRUST_LIB_H
#define __THRUST_LIB_H
#ifdef __cplusplus
extern "C" {
#endif 

#include <math.h>
//#include "matrix_lib.h"
#include "arm_math.h"

extern float32_t ctrl_data[4];

void control_data_init(void);
void control_data_calc_x(double alpha_roll_set, double alpha_pitch_set, double alpha_yaw_set,double accel_set);
void control_data_calc_z(double alpha_roll_set, double alpha_pitch_set, double alpha_yaw_set,double accel_set);


#endif
#ifdef __cplusplus
}
#endif
