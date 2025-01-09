/**
 * Frame Source File
 * 
 * @File:        .c
 * @Brief:       远程命令接收任务相关库文件
 * @Author:      Wang YiFei
 * @Modified:    2024.5.3
 *
 */
 
#include "remote_task.h"

AUV_RemoteHandleTypedef auv_remote;
AUV_HeightHandleTypedef auv_height;

//状态确认函数，该函数目前功能尚不完善，三个拨杆至少要有一个拨杆为0x01才能确认状态，待后续优化。
void auv_getstatus(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol){
//	if(telecontrol->dial[0] == 0x01){
//		remote->status = FORWARD_BACK;
//		return;
//	}
//	if(telecontrol->dial[1] == 0x01){
//		remote->status = LEFT_RIGHT;
//		return;
//	}
//	if(telecontrol->dial[2] == 0x01){
//		remote->status = UP_DOWN;
//		return;
//	}
//	return;
	//在这里进行一次修改，为了适应增加的调平功能
	if(telecontrol->dial[0] == 0x01 && telecontrol->dial[1] == 0x01){
		remote->status = UP_DOWN;
		return;
	}
	if(telecontrol->dial[0] == 0x01){
		remote->status = FORWARD_BACK;
		return;
	}
	if(telecontrol->dial[1] == 0x01){
		remote->status = LEFT_RIGHT;
		return;
	}
	
}

float speed_temp;
float speed_temp_2;
void auv_setspeedref(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol){
	speed_temp = uint6_cov_float(telecontrol->rocker[2]) * 10000000.0f;
	speed_temp_2 = uint6_cov_float(telecontrol->rocker[0]) * 10000000.0f;
	
	
	if(remote->status == FORWARD_BACK){
		if(speed_temp >= MIDDLE_ERR){
			remote->speed_ref_x = data_calc(speed_temp,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
			remote->speed_ref_y = PROPELLER_MIDDLE;
		}else{
			remote->speed_ref_x = data_calc(speed_temp,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
			remote->speed_ref_y = PROPELLER_MIDDLE;
		}
		
		if(telecontrol->dial[2] == 0x01){
				if(speed_temp_2 >= MIDDLE_ERR){
				//remote->speed_ref_x = data_calc(speed_temp,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
				remote->speed_ref_y = data_calc(speed_temp_2,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
			}else{
				//remote->speed_ref_x = data_calc(speed_temp,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
				remote->speed_ref_y = data_calc(speed_temp_2,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
			}
		}
		
		
		
		if(fabs(remote->speed_ref_x + 0.3) <= 0.1){
				remote->speed_ref_x = PROPELLER_MIDDLE;
		}
		if(fabs(remote->speed_ref_y + 0.3) <= 0.1){
				remote->speed_ref_y = PROPELLER_MIDDLE;
		}

	}else if(remote->status == LEFT_RIGHT){
		if(speed_temp >= MIDDLE_ERR){
			remote->speed_ref_x = PROPELLER_MIDDLE;
			remote->speed_ref_y = data_calc(speed_temp,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
		}else{
			remote->speed_ref_x = PROPELLER_MIDDLE;
			remote->speed_ref_y = data_calc(speed_temp,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
		}
		
		if(telecontrol->dial[2] == 0x01){
				if(speed_temp_2 >= MIDDLE_ERR){
				remote->speed_ref_x = data_calc(speed_temp_2,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
				//remote->speed_ref_y = data_calc(speed_temp_2,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
			}else{
				remote->speed_ref_x = data_calc(speed_temp_2,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
				//remote->speed_ref_y = data_calc(speed_temp_2,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
			}
		}
		
		if(fabs(remote->speed_ref_x + 0.3) <= 0.1){
				remote->speed_ref_x = PROPELLER_MIDDLE;
		}
		if(fabs(remote->speed_ref_y + 0.3) <= 0.1){
				remote->speed_ref_y = PROPELLER_MIDDLE;
		}
	}else if(remote->status == UP_DOWN){
		if(speed_temp >= MIDDLE_ERR){
			remote->speed_ref_x = data_calc(speed_temp,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
			remote->speed_ref_y = data_calc(speed_temp,MAX_ERR,MIDDLE_ERR,PROPELLER_MAX,PROPELLER_MIDDLE);
		}else{
			remote->speed_ref_x = data_calc(speed_temp,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
			remote->speed_ref_y = data_calc(speed_temp,MIDDLE_ERR,MIN_ERR,PROPELLER_MIDDLE,PROPELLER_MIN);
		}
		if(fabs(remote->speed_ref_x + 0.3) <= 0.1){
				remote->speed_ref_x = PROPELLER_MIDDLE;
			}
		if(fabs(remote->speed_ref_y + 0.3) <= 0.1){
				remote->speed_ref_y = PROPELLER_MIDDLE;
			}
	}
	
	(&auv_propeller[0])->speed_ref = remote->speed_ref_x;
	(&auv_propeller[1])->speed_ref = remote->speed_ref_x;
	(&auv_propeller[2])->speed_ref = remote->speed_ref_y;
	(&auv_propeller[3])->speed_ref = remote->speed_ref_y;
	
	if(telecontrol->dial[3] == 0x01){
		(&auv_propeller[1])->speed_ref = -remote->speed_ref_x;
		(&auv_propeller[3])->speed_ref = -remote->speed_ref_y;
	}
		
		
}
//Set Height Control Speed Reference
void auv_setspeedref_height(AUV_HeightHandleTypedef* height, float speed_x, float speed_yl, float speed_yr){
	height->speed_ref_x = speed_x;
	height->speed_ref_yleft = speed_yl;
	height->speed_ref_yright = speed_yr;
	
	(&auv_propeller[0])->speed_ref = height->speed_ref_x;
	(&auv_propeller[1])->speed_ref = -height->speed_ref_x;
	(&auv_propeller[2])->speed_ref = height->speed_ref_yleft;
	(&auv_propeller[3])->speed_ref = height->speed_ref_yright;
}

float angle_temp;
void auv_setangleref(AUV_RemoteHandleTypedef* remote, telecontrol_struct_t* telecontrol){
	angle_temp = uint6_cov_float(telecontrol->rocker[1]) * 10000000.0f;
	
	
	if(remote->status == FORWARD_BACK){
		remote->angle_ref_x = data_calc(angle_temp,MAX_ERR,MIN_ERR,SERVO_MAX,SERVO_MIN);
		remote->angle_ref_y = SERVO_MIDDLE;
		
		if(telecontrol->dial[2] == 0x01){
			remote->angle_ref_y = SERVO_MAX;
		}
		
	}else if(remote->status == LEFT_RIGHT){
		remote->angle_ref_x = SERVO_MIDDLE;
		remote->angle_ref_y = data_calc(angle_temp,MAX_ERR,MIN_ERR,SERVO_MAX,SERVO_MIN);
		
		if(telecontrol->dial[2] == 0x01){
			remote->angle_ref_x = SERVO_MAX;
		}
		
	}else if(remote->status == UP_DOWN){
		remote->angle_ref_x = data_calc(angle_temp,MAX_ERR,MIN_ERR,SERVO_MAX,SERVO_MIN);
		remote->angle_ref_y = data_calc(angle_temp,MAX_ERR,MIN_ERR,SERVO_MAX,SERVO_MIN);
		
	}
	
	(&auv_servo[0])->angle_ref = remote->angle_ref_x;
	(&auv_servo[1])->angle_ref = remote->angle_ref_x;
	(&auv_servo[2])->angle_ref = remote->angle_ref_y;
	(&auv_servo[3])->angle_ref = remote->angle_ref_y;
	if(telecontrol->dial[3] == 0x01){
		(&auv_servo[1])->angle_ref = PI -(&auv_servo[1])->angle_ref;
		(&auv_servo[3])->angle_ref = PI -(&auv_servo[3])->angle_ref;
	}
		
		
}

void auv_setangleref_height(AUV_HeightHandleTypedef* height,float angle_x){
	height->angle_ref_x = angle_x;
	height->angle_ref_y = SERVO_MIDDLE;
	
	(&auv_servo[0])->angle_ref = height->angle_ref_x;
	(&auv_servo[1])->angle_ref = PI - height->angle_ref_x;
	(&auv_servo[2])->angle_ref = height->angle_ref_y;
	(&auv_servo[3])->angle_ref = height->angle_ref_y;
}

void auv_control(){
	for(int i = 0;i < 4;i++){
		if(fabs((&auv_propeller[i])->speed_ref - (&auv_propeller[i])->speed_calc) > 0.2){
		(&auv_propeller[i])->speed_calc = (&auv_propeller[i])->speed_ref;
		}
		
		if(fabs((&auv_servo[i])->angle_ref - (&auv_servo[i])->angle_calc) > 0.2){	
		(&auv_servo[i])->angle_calc = (&auv_servo[i])->angle_ref;
		}
		
		propeller_calcspeed(&auv_propeller[i]);
		servo_calcangle(&auv_servo[i]);
	}
}

void auv_control_height(){
	for(int i = 0;i < 4;i++){
		(&auv_propeller[i])->speed_calc = (&auv_propeller[i])->speed_ref;
		
		(&auv_servo[i])->angle_calc = (&auv_servo[i])->angle_ref;
		
		propeller_calcspeed(&auv_propeller[i]);
		servo_calcangle(&auv_servo[i]);
	}
}