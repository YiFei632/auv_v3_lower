/**
 * Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#ifndef __MATHTOOL_LIB_H
#define __MATHTOOL_LIB_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "stdint.h"	
	
union Fp32
{
    uint32_t u;
    float f;
};



float data_calc(float data,float max_data,float min_data,float max_pwm,float min_pwm);
uint16_t float_cov_uint16(float value);
float uint6_cov_float(uint16_t value);

#endif
#ifdef __cplusplus
}
#endif
