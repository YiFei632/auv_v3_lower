#include "sim_I2C.h"

void delay_us(__IO uint32_t delay)
{
	int last, curr, val;
	int temp;

	while (delay != 0)
	{
		temp = delay > 900 ? 900 : delay;
		last = SysTick->VAL;
		curr = last - CPU_FREQUENCY_MHZ * temp;
		if (curr >= 0)
		{
			do
			{
				val = SysTick->VAL;
			}
			while ((val < last) && (val >= curr));
		}
		else
		{
			curr += CPU_FREQUENCY_MHZ * 1000;
			do
			{
				val = SysTick->VAL;
			}
			while ((val <= last) || (val > curr));
		}
		delay -= temp;
	}
}

void delay_i2c(sim_I2C_t * i2c)
{
	delay_us(i2c->delay_time);
}

/**********************************************************
   1.IIC���ģ��   ʹ��HAL��ʱ
   2.��ҪSTM32CubeMX���ó�ʼ�����������ΪGPIOģʽ SDA SCL��ʼ״̬�¶������ ���� ����ģʽ 
   4.��ʼ״̬��SDA �� SCLҪ���ߵ�ƽ ʹ�øߵ͵�ƽת��ʱ֮��Ҫ�����Ե�us����ʱ
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.�������� �궨����IF��� 
2.�����ŵ�ƽ����Ҫ�����ģʽ
3.SCLһֱ�������ģʽ(���ʱ�ӿ϶������ģʽ)
4.�궨�������SDA��SCL
**********************************************************/
//#define SCL_Type    GPIOA 	//ICM42688_SCL_GPIO_Port
//#define SDA_Type    GPIOC 	//ICM42688_SDA_GPIO_Port
// 
//#define SCL_GPIO    GPIO_PIN_12				//ICM42688_SCL_Pin
//#define SDA_GPIO    GPIO_PIN_1			//ICM42688_SDA_Pin

//��������ߵ͵�ƽģʽ
#define SDA_OUT(i2c, X)   if(X) \
					 HAL_GPIO_WritePin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN, GPIO_PIN_RESET);
 
#define SCL_OUT(i2c, X)   if(X) \
					 HAL_GPIO_WritePin(i2c->SCL.GPIO_PORT, i2c->SCL.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(i2c->SCL.GPIO_PORT, i2c->SCL.GPIO_PIN, GPIO_PIN_RESET);    
                                     
#define SDA_IN(i2c)		 HAL_GPIO_ReadPin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN)//ֻ������ģʽ���ܶ�ȡ��ƽ״̬
						 
/*****************************************
  SDA����ת��Ϊ OUT���ģʽ(���ģʽ��ֹͣ ��ʼ�ź�) 
******************************************/
void IIC_SDA_Mode_OUT(sim_I2C_t * i2c)
{
  GPIO_InitStruct.Pin = i2c->SDA.GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c->SDA.GPIO_PORT, &GPIO_InitStruct);
}
/*****************************************
  SDA����ת��Ϊ ����ģʽ(����ģʽ������������) 
******************************************/
void IIC_SDA_Mode_IN(sim_I2C_t * i2c)
{
  GPIO_InitStruct.Pin = i2c->SDA.GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c->SDA.GPIO_PORT, &GPIO_InitStruct);
}

/*****************************************
  IIC��ʼ�ź�
******************************************/
void IIC_Start(sim_I2C_t * i2c)//IIC��ʼ�ź�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT(i2c);
	
	//����״̬���������Ǹߵ�ƽ
	SDA_OUT(i2c, 1);
	SCL_OUT(i2c, 1) ;
	delay_i2c(i2c);
	
	//����������
	SDA_OUT(i2c, 0);
	delay_i2c(i2c);
	
	//������ʱ����
	SCL_OUT(i2c, 0) ;
	delay_i2c(i2c);
}
//IICֹͣ�ź�
void IIC_Stop(sim_I2C_t * i2c)
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT(i2c);
 
	//����
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	//ʱ����������
	SCL_OUT(i2c, 1);
	delay_i2c(i2c);
	
	//�ٰ�����������
	SDA_OUT(i2c, 1);
	delay_i2c(i2c);
}

void IIC_Send_Byte(sim_I2C_t * i2c, uint8_t d)//��������8λ���ݸ��ӻ�MSB ��λ�ȷ�
{
    uint8_t i = 0;
    	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT(i2c);
	
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//��ʾ������1
			SDA_OUT(i2c, 1)
		else
			SDA_OUT(i2c, 0);
		
		delay_i2c(i2c);
		SCL_OUT(i2c, 1);//����ʱ���ߣ����߶Է�����Զ���
		
		delay_i2c(i2c);
		SCL_OUT(i2c, 0);//����ʱ���ߣ����߶Է�����ʱ���������׼������
	}
 
}
uint8_t IIC_Wait_Ack(sim_I2C_t * i2c)//�ȴ��ӻ�������Ӧ����߲�Ӧ��
{
	delay_i2c(i2c);
	uint8_t ack = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN(i2c);
	
	//ʱ��������,ʱ����Ϊ�ߵ�ƽ�ڼ䣬���������ݻ���ack������Ч��
	SCL_OUT(i2c, 1);
	delay_i2c(i2c);
	
	if( SDA_IN(i2c)  == 1)
		ack = 1;//��ЧACK��������ЧӦ��
	else
		ack = 0;//��ЧACK��������ЧӦ��
	
	SCL_OUT(i2c, 0);
    
	delay_i2c(i2c);
	return ack;
}


uint8_t  IIC_Read_Byte(sim_I2C_t * i2c)//�ӻ�����8λ���ݸ�����
{
	uint8_t i =0;
	uint8_t data = 0;
	//����Ϊ����ģʽ
	IIC_SDA_Mode_IN(i2c);
	//������ʱ���ߣ�׼������
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(i2c, 1);//ʱ����Ϊ�ߵ�ƽ�ڼ����ݲ�����Ч��
		delay_i2c(i2c);
		if(SDA_IN(i2c) == 1)
			data |= (0x1<<(7-i));//���ݾ���1
		else
			data &= ~(0x1<<(7-i));//���ݾ���0
		
		SCL_OUT(i2c, 0);//���߶Է���ʱ׼�����ݣ��ȱ��д
		delay_i2c(i2c);
	}
	return data;
}
void IIC_Ack(sim_I2C_t * i2c, uint8_t ack)//��������Ӧ����߲�Ӧ����ӻ�
{
	//����Ϊ���ģʽ
	IIC_SDA_Mode_OUT(i2c);
	
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	SDA_OUT(i2c, ack) ;//���͸�/�͵�ƽ--->���Ͳ�Ӧ��/Ӧ��
	delay_i2c(i2c);
	
	SCL_OUT(i2c, 1);//���ߴӻ����Ѿ�׼�������ݣ�����Զ�ȡ��
	delay_i2c(i2c);
	
	SCL_OUT (i2c, 0);//����ʱ���ߣ�����ack����
	delay_i2c(i2c);
}

uint8_t IIC_ReadOneByte(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr)
{                  
	uint8_t temp=-1;                                                                                   
  IIC_Start(i2c);  
	IIC_Send_Byte(i2c, DeviceAddress|0x00);       //����ط�0X68���豸��ַ��ICM42688��0X68��AS5600��0X36, WF5803��0xDA  IIC_Send_Byte((0X68<<1)|0x00); 
	i2c->ack[0]=IIC_Wait_Ack(i2c); 
	
  IIC_Send_Byte(i2c, ReadAddr);   //
	i2c->ack[1]=IIC_Wait_Ack(i2c);  
	
	IIC_Start(i2c);              
	IIC_Send_Byte(i2c, DeviceAddress|0x01);           //           
	i2c->ack[2]=IIC_Wait_Ack(i2c);     
  temp=IIC_Read_Byte(i2c);           
  IIC_Stop(i2c);//       
	return temp;
}

void IIC_ReadBytes(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr, uint8_t num, uint8_t * data)
{                                                                                   
  IIC_Start(i2c);  
	IIC_Send_Byte(i2c, DeviceAddress|0x00);       //����ط�0X68���豸��ַ��ICM42688��0X68��AS5600��0X36, WF5803��0xDA  IIC_Send_Byte((0X68<<1)|0x00); 
	i2c->ack[0]=IIC_Wait_Ack(i2c); 
	
  IIC_Send_Byte(i2c, ReadAddr);   //
	i2c->ack[1]=IIC_Wait_Ack(i2c);  
	
	IIC_Start(i2c);              
	IIC_Send_Byte(i2c, DeviceAddress|0x01);           //           
	i2c->ack[2]=IIC_Wait_Ack(i2c); 
	for(int i = 0; i < num; i++)
	{
		data[i]=IIC_Read_Byte(i2c); 
		IIC_Ack(i2c, 0);
	}         
  IIC_Stop(i2c);//       
}

void IIC_WriteOneByte(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr, uint8_t data_byte)
{	
	IIC_Start(i2c);
	IIC_Send_Byte(i2c, DeviceAddress|0x00);
	i2c->ack[0]=IIC_Wait_Ack(i2c);
	IIC_Send_Byte(i2c, ReadAddr);
	i2c->ack[1]=IIC_Wait_Ack(i2c);
	IIC_Send_Byte(i2c, data_byte);
	i2c->ack[2]=IIC_Wait_Ack(i2c);
	IIC_Stop(i2c);
}

uint8_t IIC_Test(sim_I2C_t * i2c, uint8_t * addr)
{
	for(uint8_t i = 0; i < 0x80; i++)
	{
		IIC_Start(i2c);
		IIC_Send_Byte(i2c, (i << 1)|0x00);
		uint8_t temp = IIC_Wait_Ack(i2c); 
		if(temp == 0)
		{
			*addr = i;
			return 1;
		}
		IIC_Stop(i2c);
		delay_i2c(i2c);
	}
}