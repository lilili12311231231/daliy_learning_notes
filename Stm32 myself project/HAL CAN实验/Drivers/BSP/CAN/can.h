#ifndef __CAN_H__
#define __CAN_H__

#include "./SYSTEM/sys/sys.h"

// CAN 引脚定义
/******************************************************************************************/
#define CAN_RX_GPIO_PORT GPIOA
#define CAN_RX_GPIO_PIN GPIO_PIN_11
#define CAN_RX_GPIO_CLK_ENABLE() do{__HAL_RCC_GPIOA_CLK_ENABLE();}while(0) /* 使能能GPIOA时钟 */

#define CAN_TX_GPIO_PORT GPIOA
#define CAN_TX_GPIO_PIN GPIO_PIN_12
#define CAN_TX_GPIO_CLK_ENABLE() do{__HAL_RCC_GPIOA_CLK_ENABLE();}while(0) /* 使能能GPIOA时钟 */
/******************************************************************************************/
/*  CAN 接收RX0中断使能*/
#define CAN_RX0_INT_ENABLE CAN1_RX0_IRQHandler

/*函数声明*/
uint8_t can_receive_msg(uint32_t id, uint8_t *buf); /* 接收CAN消息 */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len);                           /* CAN发送数据 */
uint8_t can_init(uint32_t tsjw,uint32_t tbs2,uint32_t tbs1,uint16_t brp,uint32_t mode); /* CAN初始化 */

#endif