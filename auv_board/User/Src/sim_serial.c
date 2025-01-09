#include "sim_serial.h"

sim_serial_t serial1 = //????????RF200????????,??425MHZ?????,115200?????
{
	.TX = 
	{
		.GPIO_PORT = serial1_TX_GPIO_Port,
		.GPIO_PIN = serial1_TX_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.RX = 
	{
		.GPIO_PORT = serial1_RX_GPIO_Port,
		.GPIO_PIN = serial1_RX_Pin,
		.defalt_state = GPIO_PIN_SET
	},
	.delay_time = 25//??8.7ms
};

#define SET_TX(serial, X)   if(X) \
					 HAL_GPIO_WritePin(serial->TX.GPIO_PORT, serial->TX.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(serial->TX.GPIO_PORT, serial->TX.GPIO_PIN, GPIO_PIN_RESET);
#define SET_RX(serial, X)   if(X) \
					 HAL_GPIO_WritePin(serial->RX.GPIO_PORT, serial->RX.GPIO_PIN, GPIO_PIN_SET); \
				     else  \
					 HAL_GPIO_WritePin(serial->RX.GPIO_PORT, serial->RX.GPIO_PIN, GPIO_PIN_RESET);

void delay_0_1_us(__IO uint32_t delay)
{
	int last, curr, val;
	int temp;

	while (delay != 0)
	{
		temp = delay > 9000 ? 9000 : delay;
		last = SysTick->VAL;
		curr = last - CPU_FREQUENCY_MHZ * temp / 10;
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

void serial_send_byte(sim_serial_t * serial, uint8_t data) 
{
	SET_TX(serial, 0);//????
	//delay_0_1_us(serial->delay_time);//????115200
	//delay_us(serial->delay_time);
	delay_us(104);//baudrate:9600
	uint8_t count = 0;
	//????
	while(count < 8) {
		if (data & 0x01)
		{
			SET_TX(serial, 1);
		}
		else
		{
			SET_TX(serial, 0);
		}
		//delay_us(serial->delay_time);
		delay_us(104);
		data >>= 1;
		count++;
	}
	SET_TX(serial, 1);//????
	//delay_us(serial->delay_time);
	delay_us(104);
}
 
void serial_send_str(sim_serial_t * serial, char *str) 
{
	while (*str != '\0') {
		serial_send_byte(serial, *str);
		str++;
	}
}

void serial_send_data(sim_serial_t *serial, uint8_t *data){
	//uint16_t datasize = sizeof(data) / sizeof(data[0]);
	uint16_t datasize = 16;
	for(int i = 0;i < datasize;i++){
		serial_send_byte(serial,data[i]);
	}
}

void serial_send_formatted(sim_serial_t *serial, const char *format, ...) 
{
	char buffer[256];  // ?????????,????????????
	va_list args;      // ???? va_list ?????,??????????
	va_start(args, format);// ?????????
	vsnprintf(buffer, sizeof(buffer), format, args);// ?? vsnprintf ????????????
	va_end(args);// ?????????
	serial_send_str(serial, buffer);// ??????????????????
}