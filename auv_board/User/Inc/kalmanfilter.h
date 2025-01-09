#ifndef __KALMAN_H
#define __KALMAN_H 

//¿¨¶ûÂü½âËã·¨¿â
#include "main.h"
#include "sim_SPItwo.h"
#include <math.h>

void Angle_Calcu(void);
void Kalman_Filter_X(float Accel,float Gyro);
void Kalman_Filter_Y(float Accel,float Gyro);


#endif