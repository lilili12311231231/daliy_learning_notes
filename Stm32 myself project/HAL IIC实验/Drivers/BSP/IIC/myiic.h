#ifndef __MYIIC_H
#define __MYIIC_H

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
/* 引脚 定义 */
#define IIC_SCL_GPIO_PORT GPIOB/* SCL端口 */
#define IIC_SCL_GPIO_PIN GPIO_PIN_8/* SCL引脚 */
/* PB 口时钟使能 */
#define IIC_SCL_GPIO_CLK_ENABLE()do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)
#define IIC_SDA_GPIO_PORT GPIOB/* SDA端口 */
#define IIC_SDA_GPIO_PIN GPIO_PIN_9/* SDA引脚 */
/* PB 口时钟使能 */
#define IIC_SDA_GPIO_CLK_ENABLE()do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)
/* IO 操作 */
#define IIC_SCL(x)  do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN,\
  GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN,\
  GPIO_PIN_RESET); }while(0)
  #define IIC_SDA(x)  do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN,\
  GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN,\
  GPIO_PIN_RESET); }while(0)
/* 读取SDA引脚状态 */
#define IIC_READ_SDA HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)
/* IIC所有操作函数 */
void iic_init(void); /* 初始化IIC的IO口 */
void iic_start(void); /* 发送IIC起始信号 */
void iic_stop(void); /* 发送IIC停止信号 */
void iic_ack(void); /* IIC发送ACK信号 */
void iic_nack(void); /* IIC发送NACK信号 */
uint8_t iic_wait_ack(void); /* 等待ACK信号 */
void iic_send_byte(uint8_t txd); /* IIC发送一个字节 */
uint8_t iic_read_byte(unsigned char ack); /* IIC读取一个字节 */

#endif
