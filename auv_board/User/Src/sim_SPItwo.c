/* USER CODE BEGIN 0 */
#include "stm32h7xx_hal.h"
#include "gpio.h" 
#include "main.h"
#include "sim_SPItwo.h"


// Function to initialize GPIO pins for SPI
void MX_SPI1_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Clock enable for GPIO ports
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // SCK pin configuration
  GPIO_InitStruct.Pin = ICM42688_SPI_CK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(ICM42688_SPI_CK_GPIO_Port, &GPIO_InitStruct);

  // MISO pin configuration
  GPIO_InitStruct.Pin = ICM42688_SPI_MISO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ICM42688_SPI_MISO_GPIO_Port, &GPIO_InitStruct);

  // MOSI pin configuration
  GPIO_InitStruct.Pin = ICM42688_SPI_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ICM42688_SPI_MOSI_GPIO_Port, &GPIO_InitStruct);

  // NSS pin configuration
  GPIO_InitStruct.Pin = ICM42688_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ICM42688_SPI_CS_GPIO_Port, &GPIO_InitStruct);
}
/**
*
*使用了大量的 HAL_GetTick() 和 HAL_Delay()，特别是在 SPI 的位传输过程中，这可能会导致系统时基问题
*因此增加了SPI_Delay函数 从而解决了该问题
*修改了SPI1_Transmit、SPI1_Receive函数
*
**/

static inline void SPI_Delay(uint32_t count)
{
    while(count--);
}

// Function to transmit and receive SPI data using GPIO
void SPI1_TransmitReceive(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t length)
{
  uint16_t i;

  // Set NSS low to start communication
  HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);

  for (i = 0; i < length; i++)
  {
    // Clock the data out on MOSI
    HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ICM42688_SPI_MOSI_GPIO_Port, ICM42688_SPI_MOSI_Pin, (txBuffer[i] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_SET);

    // Read the data in on MISO
    rxBuffer[i] = (HAL_GPIO_ReadPin(ICM42688_SPI_MISO_GPIO_Port, ICM42688_SPI_MISO_Pin) == GPIO_PIN_SET) ? 0xFF : 0x00;

    // Shift the tx data left by one bit
    txBuffer[i] <<= 1;
  }

  // Set NSS high to end communication
  HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);
}
// SPI1 Transmit function
/*
void SPI1_Transmit(uint8_t *txBuffer, uint16_t length, uint32_t timeout)
{
    uint32_t start_time = HAL_GetTick();

    for (uint16_t i = 0; i < length; i++)
    {
        // Check for timeout
        if ((HAL_GetTick() - start_time) > timeout)
        {
            return; // Timeout occurred, return
        }

        // Clock the data out on MOSI
        HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ICM42688_SPI_MOSI_GPIO_Port, ICM42688_SPI_MOSI_Pin, (txBuffer[i] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_Delay(1); // Delay for proper timing
        HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_SET);
        HAL_Delay(1); // Delay for proper timing

        // Shift the tx data left by one bit
        txBuffer[i] <<= 1;
    }
}*/
// 在 SPI1_Transmit 函数中使用
void SPI1_Transmit(uint8_t *txBuffer, uint16_t length, uint32_t timeout)
{
    uint32_t start_time = HAL_GetTick();

    for (uint16_t i = 0; i < length; i++)
    {
        for(uint8_t bit = 0; bit < 8; bit++)  // 每次处理一位
        {
            // 设置 MOSI
            HAL_GPIO_WritePin(ICM42688_SPI_MOSI_GPIO_Port, ICM42688_SPI_MOSI_Pin, 
                            (txBuffer[i] & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
            
            SPI_Delay(2);  // 很短的延时
            
            // 时钟上升沿
            HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_SET);
            SPI_Delay(2);
            
            // 时钟下降沿
            HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_RESET);
            
            txBuffer[i] <<= 1;  // 移位到下一位
        }

        if ((HAL_GetTick() - start_time) > timeout)
            return;
    }
}

// SPI1 Receive function
/*
void SPI1_Receive(uint8_t *rxBuffer, uint16_t length, uint32_t timeout)
{
    uint32_t start_time = HAL_GetTick();

    for (uint16_t i = 0; i < length; i++)
    {
        // Check for timeout
        if ((HAL_GetTick() - start_time) > timeout)
        {
            return; // Timeout occurred, return
        }

        // Clock the data in on MISO
        HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_RESET);
        rxBuffer[i] = (HAL_GPIO_ReadPin(ICM42688_SPI_MISO_GPIO_Port, ICM42688_SPI_MISO_Pin) == GPIO_PIN_SET) ? 0xFF : 0x00;
        HAL_Delay(1); // Delay for proper timing
        HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_SET);
        HAL_Delay(1); // Delay for proper timing
    }
}*/
void SPI1_Receive(uint8_t *rxBuffer, uint16_t length, uint32_t timeout)
{
    uint32_t start_time = HAL_GetTick();

    for (uint16_t i = 0; i < length; i++)
    {
        rxBuffer[i] = 0;  // 清零接收缓冲
        for(uint8_t bit = 0; bit < 8; bit++)
        {
            // 时钟上升沿
            HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_SET);
            SPI_Delay(2);
            
            // 读取数据
            rxBuffer[i] = (rxBuffer[i] << 1) | 
                         (HAL_GPIO_ReadPin(ICM42688_SPI_MISO_GPIO_Port, ICM42688_SPI_MISO_Pin) == GPIO_PIN_SET ? 1 : 0);
            
            // 时钟下降沿
            HAL_GPIO_WritePin(ICM42688_SPI_CK_GPIO_Port, ICM42688_SPI_CK_Pin, GPIO_PIN_RESET);
            SPI_Delay(2);
        }

        if ((HAL_GetTick() - start_time) > timeout)
            return;
    }
}

// 写入寄存器数据
static void Write_Data_ICM42688(uint8_t reg, uint8_t dat)
{
    uint8_t send_data[2];
    send_data[0] = reg & 0x7F;    // 清除最高位，表示写操作
    send_data[1] = dat;

    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低，开始通信
    
    SPI1_Transmit(send_data, 2, HAL_MAX_DELAY);  // 发送寄存器地址和数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高，结束通信
}

// 读取多个寄存器数据
static void Read_Datas_ICM42688(uint8_t reg, uint8_t *dat, uint16_t num)
{
    reg |= 0x80;    // 设置最高位，表示读操作
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_RESET);  // CS拉低，开始通信
    
    SPI1_Transmit(&reg, 1, HAL_MAX_DELAY);      // 发送寄存器地址
    SPI1_Receive(dat, num, HAL_MAX_DELAY);      // 读取数据
    
    HAL_GPIO_WritePin(ICM42688_SPI_CS_GPIO_Port, ICM42688_SPI_CS_Pin, GPIO_PIN_SET);    // CS拉高，结束通信
}

