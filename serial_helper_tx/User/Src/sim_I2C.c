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
   1.IIC软件模拟   使用HAL库时
   2.需要STM32CubeMX配置初始化的相关引脚为GPIO模式 SDA SCL初始状态下都是输出 推挽 上拉模式 
   4.初始状态下SDA 与 SCL要给高电平 使用高低电平转换时之间要有明显的us级延时
**********************************************************/
static GPIO_InitTypeDef GPIO_InitStruct;
/**********************************************************
1.引脚配置 宏定义用IF语句 
2.给引脚电平必须要给输出模式
3.SCL一直都是输出模式(输出时钟肯定是输出模式)
4.宏定义绑定引脚SDA与SCL
**********************************************************/
//#define SCL_Type    GPIOA 	//ICM42688_SCL_GPIO_Port
//#define SDA_Type    GPIOC 	//ICM42688_SDA_GPIO_Port
// 
//#define SCL_GPIO    GPIO_PIN_12				//ICM42688_SCL_Pin
//#define SDA_GPIO    GPIO_PIN_1			//ICM42688_SDA_Pin

//设置输出高低电平模式
#define SDA_OUT(i2c, X)   if(X) \
					 HAL_GPIO_WritePin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN, GPIO_PIN_RESET);
 
#define SCL_OUT(i2c, X)   if(X) \
					 HAL_GPIO_WritePin(i2c->SCL.GPIO_PORT, i2c->SCL.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(i2c->SCL.GPIO_PORT, i2c->SCL.GPIO_PIN, GPIO_PIN_RESET);    
                                     
#define SDA_IN(i2c)		 HAL_GPIO_ReadPin(i2c->SDA.GPIO_PORT, i2c->SDA.GPIO_PIN)//只有输入模式才能读取电平状态
						 
/*****************************************
  SDA引脚转变为 OUT输出模式(输出模式给停止 开始信号) 
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
  SDA引脚转变为 输入模式(输入模式传输具体的数据) 
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
  IIC开始信号
******************************************/
void IIC_Start(sim_I2C_t * i2c)//IIC开始信号
{
	//设置为输出模式
	IIC_SDA_Mode_OUT(i2c);
	
	//空闲状态两个引脚是高电平
	SDA_OUT(i2c, 1);
	SCL_OUT(i2c, 1) ;
	delay_i2c(i2c);
	
	//拉低数据线
	SDA_OUT(i2c, 0);
	delay_i2c(i2c);
	
	//再拉低时钟线
	SCL_OUT(i2c, 0) ;
	delay_i2c(i2c);
}
//IIC停止信号
void IIC_Stop(sim_I2C_t * i2c)
{
	//设置为输出模式
	IIC_SDA_Mode_OUT(i2c);
 
	//拉低
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	//时钟线先拉高
	SCL_OUT(i2c, 1);
	delay_i2c(i2c);
	
	//再把数据线拉高
	SDA_OUT(i2c, 1);
	delay_i2c(i2c);
}

void IIC_Send_Byte(sim_I2C_t * i2c, uint8_t d)//主机发送8位数据给从机MSB 高位先发
{
    uint8_t i = 0;
    	//设置为输出模式
	IIC_SDA_Mode_OUT(i2c);
	
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	for(i=0;i<8;i++)
	{
		if(d & (0x1<<(7-i)))//表示数据是1
			SDA_OUT(i2c, 1)
		else
			SDA_OUT(i2c, 0);
		
		delay_i2c(i2c);
		SCL_OUT(i2c, 1);//拉高时钟线，告诉对方你可以读了
		
		delay_i2c(i2c);
		SCL_OUT(i2c, 0);//拉低时钟线，告诉对方你暂时别读，我在准备数据
	}
 
}
uint8_t IIC_Wait_Ack(sim_I2C_t * i2c)//等待从机给主机应答或者不应答
{
	delay_i2c(i2c);
	uint8_t ack = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN(i2c);
	
	//时钟线拉高,时钟线为高电平期间，不管是数据还是ack都是有效的
	SCL_OUT(i2c, 1);
	delay_i2c(i2c);
	
	if( SDA_IN(i2c)  == 1)
		ack = 1;//无效ACK，就是无效应答
	else
		ack = 0;//有效ACK，就是有效应答
	
	SCL_OUT(i2c, 0);
    
	delay_i2c(i2c);
	return ack;
}


uint8_t  IIC_Read_Byte(sim_I2C_t * i2c)//从机发送8位数据给主机
{
	uint8_t i =0;
	uint8_t data = 0;
	//设置为输入模式
	IIC_SDA_Mode_IN(i2c);
	//先拉低时钟线，准备数据
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	for(i=0;i<8;i++)
	{
		SCL_OUT(i2c, 1);//时钟线为高电平期间数据才是有效的
		delay_i2c(i2c);
		if(SDA_IN(i2c) == 1)
			data |= (0x1<<(7-i));//数据就是1
		else
			data &= ~(0x1<<(7-i));//数据就是0
		
		SCL_OUT(i2c, 0);//告诉对方此时准备数据，先别读写
		delay_i2c(i2c);
	}
	return data;
}
void IIC_Ack(sim_I2C_t * i2c, uint8_t ack)//主机发送应答或者不应答给从机
{
	//设置为输出模式
	IIC_SDA_Mode_OUT(i2c);
	
	SDA_OUT(i2c, 0);
	SCL_OUT(i2c, 0);
	delay_i2c(i2c);
	
	SDA_OUT(i2c, ack) ;//发送高/低电平--->发送不应答/应答
	delay_i2c(i2c);
	
	SCL_OUT(i2c, 1);//告诉从机我已经准备好数据，你可以读取了
	delay_i2c(i2c);
	
	SCL_OUT (i2c, 0);//拉低时钟线，发送ack结束
	delay_i2c(i2c);
}

uint8_t IIC_ReadOneByte(sim_I2C_t * i2c, uint8_t DeviceAddress, uint8_t ReadAddr)
{                  
	uint8_t temp=-1;                                                                                   
  IIC_Start(i2c);  
	IIC_Send_Byte(i2c, DeviceAddress|0x00);       //这个地方0X68是设备地址，ICM42688是0X68，AS5600是0X36, WF5803是0xDA  IIC_Send_Byte((0X68<<1)|0x00); 
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
	IIC_Send_Byte(i2c, DeviceAddress|0x00);       //这个地方0X68是设备地址，ICM42688是0X68，AS5600是0X36, WF5803是0xDA  IIC_Send_Byte((0X68<<1)|0x00); 
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