#include "kernal.h"

time_manage_t kernal_time;//用1Mhz的IIC通信，核心函数周期2.5毫秒，处理时间不超过1毫秒

_Bool is_record;
_Bool is_storage;
_Bool has_storaged;
_Bool is_record_flash;

//_Bool want_read;
//_Bool read_info;
//_Bool read_data;

uint16_t record_num_temp1, record_num_temp2, send_num_temp;
IMU_t imu_record[record_num];
WF5803_t wf5803_record[record_num];



void kernal()
{
	
	
	
	if(E70_433MT14S.is_init)
		control_task();
//	if(IMU1.is_init)	
//			Get_ICM42688_Data();
	get_wit_imu_angle(&IMU2);
	if(WF5803.is_init)	
			wf5803_getdata();

// Give Up Recording.
//	if(is_record)
//	{
//		imu_record[record_num_temp1] = IMU;
//		wf5803_record[record_num_temp1] = WF5803;
//		record_num_temp1++;
//		if(record_num_temp1 == record_num)
//			is_storage = 1;
//	}
//	
////	SDCardCmd();
//	
//	
//	if(sd_cmd == SD_READ_INFO){
//		send_num_temp = 0;
//	}
//	
//	
//	if(sd_cmd == RECORD_DATA)
//	{
//		if(record_num_temp2 == record_num){
//			sd_cmd = SD_NO_COMMAND;
//			is_record_flash ++;
//			return;
//		}
//		imu_record[record_num_temp2] = IMU;
//		wf5803_record[record_num_temp2] = WF5803;
//		record_num_temp2++;
//		send_num_temp = 0;
//	}else if(sd_cmd == SENT_DATA)
//	{
//		if(send_num_temp == record_num_temp2 * 10){
//			sd_cmd = SD_NO_COMMAND;
//			return;
//		}
//		if(send_num_temp % 10 == 0)
//		{
//			printf("%.3f %.3f %.3f %.3f %.3f %.3f\r\n", imu_record[send_num_temp/10].acc[0],imu_record[send_num_temp/10].acc[1],imu_record[send_num_temp/10].acc[2],imu_record[send_num_temp/10].gyro[0],imu_record[send_num_temp/10].gyro[1],imu_record[send_num_temp/10].gyro[2]);
//		}
//		send_num_temp++;


	//auv_control();
	//auv_getstatus(&auv_remote,&telecontrol);
//	auv_setspeedref(&auv_remote,&telecontrol);
	//auv_setspeedref_height(&auv_height,conv3.to_float,conv1.to_float,conv2.to_float);
//	auv_setangleref(&auv_remote,&telecontrol);
	//auv_setangleref_height(&auv_height,conv4.to_float);
//	auv_setspeedref_height(&auv_height,conv1.to_float,conv3.to_float,conv4.to_float);
//	auv_setangleref_height(&auv_height,conv2.to_float);
//	auv_control_height();
	
	
	
	
}

void kernal_2(){
	
	kernal_time.start_time = HAL_GetTick();
	kernal_time.timer_dur = kernal_time.start_time - kernal_time.last_start_time;
	
	
	auv_setspeedref_height(&auv_height,conv1.to_float,conv3.to_float,conv4.to_float);
	auv_setangleref_height(&auv_height,conv2.to_float);
	auv_control_height();
	
	
	
	kernal_time.end_time = HAL_GetTick();
	kernal_time.dur_time = kernal_time.end_time - kernal_time.start_time;
	kernal_time.last_start_time = kernal_time.start_time;
	
	//serial_send_formatted(&serial1, "%.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %d\r\n",  WF5803.depth,IMU1.angle[0], IMU1.angle[1], IMU1.angle[2], auv_height.speed_ref_x, auv_height.angle_ref_x, auv_height.speed_ref_yleft, auv_height.speed_ref_yright, kernal_time.dur_time);
	serial_send_formatted(&serial1, "%.3f %.3f %.3f %.3f\r\n",  WF5803.depth,IMU2.angle[0], IMU2.angle[1], IMU2.angle[2]);
}

uint16_t temp_get_hz;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器中断函数
{
	if(htim == &htim15)//定时器中断15   1hz
	{
		LED_R_CHANGE;
		
	
	}
	if(htim == &htim16)//定时器中断16   20hz
	{
		kernal();
		
		
	}
	if(htim == &htim12)//Tier Interrupt 12 10hz
	{
		kernal_2();
	}
	if (htim->Instance == TIM17) {
    HAL_IncTick();
  }
}

void stop_all_IT()
{
	HAL_TIM_Base_Stop_IT(&htim15);
	HAL_TIM_Base_Stop_IT(&htim16);
	HAL_TIM_Base_Stop_IT(&htim12);
	
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
}

void start_all_IT()
{
	HAL_TIM_Base_Start_IT(&htim15);
	HAL_TIM_Base_Start_IT(&htim16);
	HAL_TIM_Base_Start_IT(&htim12);
	
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
	HAL_UART_Receive_IT(&huart2, &aRxBuffer_2, 1);
}