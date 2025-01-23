#include "kalmanfilter.h"

//卡尔曼解算法库
float Angle_x_temp;  		//由加速度计算的x倾斜角度
float Angle_y_temp;  		//由加速度计算的y倾斜角度
float Angle_z_temp;

//读取数据预处理
void Angle_Calcu(void)	 
{
	//加速度反正切公式计算三个轴和水平面坐标系之间的夹角
	Angle_x_temp=(atan(IMU1.acc[1]/IMU1.acc[2]))*180/3.14;
	Angle_y_temp=(atan(IMU1.acc[0]/IMU1.acc[2]))*180/3.14;
	Angle_z_temp=1;
	//判断计算后角度的正负号											
//	if(Accel_x<32764) Angle_y_temp = +Angle_y_temp;
//	if(Accel_x>32764) Angle_y_temp = -Angle_y_temp;
//	if(Accel_y<32764) Angle_x_temp = +Angle_x_temp;
//	if(Accel_y>32764) Angle_x_temp = -Angle_x_temp;
	
	//4.调用卡尔曼函数
	Kalman_Filter_X(Angle_x_temp,IMU1.gyro[0]);  //卡尔曼滤波计算X倾角
	Kalman_Filter_Y(Angle_y_temp,IMU1.gyro[1]);  //卡尔曼滤波计算Y倾角			
	//5.Yaw err using the relative angle
	//IMU1.angle[2] = IMU1.angle[2] + (IMU1.gyro[2] - 1.05) * 0.05;
	Kalman_Filter_Z(Angle_z_temp,IMU1.gyro[2]);
} 


//卡尔曼参数		
float Q_angle = 0.001;		//角度数据置信度，角度噪声的协方差
float Q_gyro  = 0.003;		//角速度数据置信度，角速度噪声的协方差  
float R_angle = 0.5;		//加速度计测量噪声的协方差
float dt      = 0.05;		//滤波算法计算周期，由定时器定时20ms
char  C_0     = 1;			//H矩阵值
float Q_bias, Angle_err;	//Q_bias:陀螺仪的偏差  Angle_err:角度偏量 
float PCt_0, PCt_1, E;		//计算的过程量
float K_0, K_1, t_0, t_1;	//卡尔曼增益  K_0:用于计算最优估计值  K_1:用于计算最优估计值的偏差 t_0/1:中间变量
float P[4] ={0,0,0,0};	//过程协方差矩阵的微分矩阵，中间变量
float PP[2][2] = { { 1, 0 },{ 0, 1 } };//过程协方差矩阵P

void Kalman_Filter_X(float Accel,float Gyro) //卡尔曼函数		
{
	//步骤一，先验估计
	//公式：X(k|k-1) = AX(k-1|k-1) + BU(k)
	//X = (angle,Q_bias)
	//A(1,1) = 1,A(1,2) = -dt
	//A(2,1) = 0,A(2,2) = 1
	IMU1.angle[0] += (Gyro - Q_bias) * dt; //状态方程,角度值等于上次最优角度加角速度减零漂后积分
	
	//步骤二，计算过程协方差矩阵的微分矩阵
	//公式：P(k|k-1)=AP(k-1|k-1)A^T + Q 
	//Q(1,1) = cov(angle,angle)	Q(1,2) = cov(Q_bias,angle)
	//Q(2,1) = cov(angle,Q_bias)	Q(2,2) = cov(Q_bias,Q_bias)
	P[0]= Q_angle - PP[0][1] - PP[1][0];
	P[1]= -PP[1][1];// 先验估计误差协方差
	P[2]= -PP[1][1];
	P[3]= Q_gyro;
	PP[0][0] += P[0] * dt;   
	PP[0][1] += P[1] * dt;   
	PP[1][0] += P[2] * dt;
	PP[1][1] += P[3] * dt;	
	
	//步骤三，计算卡尔曼增益
	//公式：Kg(k)= P(k|k-1)H^T/(HP(k|k-1)H^T+R)
	//Kg = (K_0,K_1) 对应angle,Q_bias增益
	//H = (1,0)	可由z=HX+v求出z:Accel
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	//步骤四，后验估计误差协方差
	//公式：P(k|k)=(I-Kg(k)H)P(k|k-1)
	//也可写为：P(k|k)=P(k|k-1)-Kg(k)HP(k|k-1)
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	
	//步骤五，计算最优角速度值
	//公式：X(k|k)= X(k|k-1)+Kg(k)(Z(k)-X(k|k-1))
	Angle_err = Accel - IMU1.angle[0];	//Z(k)先验估计 计算角度偏差
	IMU1.angle[0] += K_0 * Angle_err;	 //后验估计，给出最优估计值
	Q_bias        += K_1 * Angle_err;	 //后验估计，跟新最优估计值偏差
	IMU1.gyro[0] = Gyro - Q_bias;	 
}

void Kalman_Filter_Y(float Accel,float Gyro) 		
{
	IMU1.angle[1] += (Gyro - Q_bias) * dt;
	P[0]=Q_angle - PP[0][1] - PP[1][0]; 
	P[1]=-PP[1][1];
	P[2]=-PP[1][1];
	P[3]=Q_gyro;	
	PP[0][0] += P[0] * dt; 
	PP[0][1] += P[1] * dt;  
	PP[1][0] += P[2] * dt;
	PP[1][1] += P[3] * dt;	
	Angle_err = Accel - IMU1.angle[1];		
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];	
	E = R_angle + C_0 * PCt_0;	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;		
	IMU1.angle[1]	+= K_0 * Angle_err;
	Q_bias	+= K_1 * Angle_err;	 
	IMU1.gyro[1] = Gyro - Q_bias;	 
}

void Kalman_Filter_Z(float Accel,float Gyro){
	IMU1.angle[2] += (Gyro - Q_bias) * dt;
	P[0]=Q_angle - PP[0][1] - PP[1][0]; 
	P[1]=-PP[1][1];
	P[2]=-PP[1][1];
	P[3]=Q_gyro;	
	PP[0][0] += P[0] * dt; 
	PP[0][1] += P[1] * dt;  
	PP[1][0] += P[2] * dt;
	PP[1][1] += P[3] * dt;	
	Angle_err = Accel - IMU1.angle[2];		
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];	
	E = R_angle + C_0 * PCt_0;	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;		
	IMU1.angle[2]	+= K_0 * Angle_err;
	Q_bias	+= K_1 * Angle_err;	 
	IMU1.gyro[2] = Gyro - Q_bias;	 
}
