#include "kalmanfilter.h"

//���������㷨��
float Angle_x_temp;  		//�ɼ��ٶȼ����x��б�Ƕ�
float Angle_y_temp;  		//�ɼ��ٶȼ����y��б�Ƕ�
float Angle_z_temp;

//��ȡ����Ԥ����
void Angle_Calcu(void)	 
{
	//���ٶȷ����й�ʽ�����������ˮƽ������ϵ֮��ļн�
	Angle_x_temp=(atan(IMU1.acc[1]/IMU1.acc[2]))*180/3.14;
	Angle_y_temp=(atan(IMU1.acc[0]/IMU1.acc[2]))*180/3.14;
	Angle_z_temp=1;
	//�жϼ����Ƕȵ�������											
//	if(Accel_x<32764) Angle_y_temp = +Angle_y_temp;
//	if(Accel_x>32764) Angle_y_temp = -Angle_y_temp;
//	if(Accel_y<32764) Angle_x_temp = +Angle_x_temp;
//	if(Accel_y>32764) Angle_x_temp = -Angle_x_temp;
	
	//4.���ÿ���������
	Kalman_Filter_X(Angle_x_temp,IMU1.gyro[0]);  //�������˲�����X���
	Kalman_Filter_Y(Angle_y_temp,IMU1.gyro[1]);  //�������˲�����Y���			
	//5.Yaw err using the relative angle
	//IMU1.angle[2] = IMU1.angle[2] + (IMU1.gyro[2] - 1.05) * 0.05;
	Kalman_Filter_Z(Angle_z_temp,IMU1.gyro[2]);
} 


//����������		
float Q_angle = 0.001;		//�Ƕ��������Ŷȣ��Ƕ�������Э����
float Q_gyro  = 0.003;		//���ٶ��������Ŷȣ����ٶ�������Э����  
float R_angle = 0.5;		//���ٶȼƲ���������Э����
float dt      = 0.05;		//�˲��㷨�������ڣ��ɶ�ʱ����ʱ20ms
char  C_0     = 1;			//H����ֵ
float Q_bias, Angle_err;	//Q_bias:�����ǵ�ƫ��  Angle_err:�Ƕ�ƫ�� 
float PCt_0, PCt_1, E;		//����Ĺ�����
float K_0, K_1, t_0, t_1;	//����������  K_0:���ڼ������Ź���ֵ  K_1:���ڼ������Ź���ֵ��ƫ�� t_0/1:�м����
float P[4] ={0,0,0,0};	//����Э��������΢�־����м����
float PP[2][2] = { { 1, 0 },{ 0, 1 } };//����Э�������P

void Kalman_Filter_X(float Accel,float Gyro) //����������		
{
	//����һ���������
	//��ʽ��X(k|k-1) = AX(k-1|k-1) + BU(k)
	//X = (angle,Q_bias)
	//A(1,1) = 1,A(1,2) = -dt
	//A(2,1) = 0,A(2,2) = 1
	IMU1.angle[0] += (Gyro - Q_bias) * dt; //״̬����,�Ƕ�ֵ�����ϴ����ŽǶȼӽ��ٶȼ���Ư�����
	
	//��������������Э��������΢�־���
	//��ʽ��P(k|k-1)=AP(k-1|k-1)A^T + Q 
	//Q(1,1) = cov(angle,angle)	Q(1,2) = cov(Q_bias,angle)
	//Q(2,1) = cov(angle,Q_bias)	Q(2,2) = cov(Q_bias,Q_bias)
	P[0]= Q_angle - PP[0][1] - PP[1][0];
	P[1]= -PP[1][1];// ����������Э����
	P[2]= -PP[1][1];
	P[3]= Q_gyro;
	PP[0][0] += P[0] * dt;   
	PP[0][1] += P[1] * dt;   
	PP[1][0] += P[2] * dt;
	PP[1][1] += P[3] * dt;	
	
	//�����������㿨��������
	//��ʽ��Kg(k)= P(k|k-1)H^T/(HP(k|k-1)H^T+R)
	//Kg = (K_0,K_1) ��Ӧangle,Q_bias����
	//H = (1,0)	����z=HX+v���z:Accel
	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];
	E = R_angle + C_0 * PCt_0;
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	//�����ģ�����������Э����
	//��ʽ��P(k|k)=(I-Kg(k)H)P(k|k-1)
	//Ҳ��дΪ��P(k|k)=P(k|k-1)-Kg(k)HP(k|k-1)
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];
	PP[0][0] -= K_0 * t_0;		
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
	
	//�����壬�������Ž��ٶ�ֵ
	//��ʽ��X(k|k)= X(k|k-1)+Kg(k)(Z(k)-X(k|k-1))
	Angle_err = Accel - IMU1.angle[0];	//Z(k)������� ����Ƕ�ƫ��
	IMU1.angle[0] += K_0 * Angle_err;	 //������ƣ��������Ź���ֵ
	Q_bias        += K_1 * Angle_err;	 //������ƣ��������Ź���ֵƫ��
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
