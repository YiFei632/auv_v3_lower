/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       存放一些用得比较多的数学计算库文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
 #include "mathtool_lib.h"
 
 float data_calc(float data,float max_data,float min_data,float max_pwm,float min_pwm){
	if(data > max_data){
		return max_pwm;
	}else if(data < min_data){
		return min_pwm;
	}else{
		float data_pwm;
		data_pwm = (float)(((data - min_data) / (max_data - min_data)) * (max_pwm - min_pwm) + min_pwm);
		return data_pwm;
	}
}
 
uint16_t float_cov_uint16(float value)
{

    const union Fp32 f32infty = { 255U << 23 };
    const union Fp32 f16infty = { 31U << 23 };
    const union Fp32 magic = { 15U << 23 };
    const uint32_t sign_mask = 0x80000000U;
    const uint32_t round_mask = ~0xFFFU;

    union Fp32 in;
    uint16_t out;

    in.f = value;

    uint32_t sign = in.u & sign_mask;
    in.u ^= sign;

    if (in.u >= f32infty.u) /* Inf or NaN (all exponent bits set) */
    {
        /* NaN->sNaN and Inf->Inf */
        out = (in.u > f32infty.u) ? 0x7FFFU : 0x7C00U;
    }
    else /* (De)normalized number or zero */
    {
        in.u &= round_mask;
        in.f *= magic.f;
        in.u -= round_mask;
        if (in.u > f16infty.u)
        {
            in.u = f16infty.u; /* Clamp to signed infinity if overflowed */
        }

        out = (uint16_t)(in.u >> 13); /* Take the bits! */
    }

    out = (uint16_t)(out | (sign >> 16));

    return out;
}

float uint6_cov_float(uint16_t value)
{
    const union Fp32 magic = { (254U - 15U) << 23 };
    const union Fp32 was_infnan = { (127U + 16U) << 23 };
    union Fp32 out;

    out.u = (value & 0x7FFFU) << 13;   /* exponent/mantissa bits */
    out.f *= magic.f;                  /* exponent adjust */
    if (out.f >= was_infnan.f)         /* make sure Inf/NaN survive */
    {
        out.u |= 255U << 23;
    }
    out.u |= (value & 0x8000U) << 16;  /* sign bit */

    return out.f;
}

