#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

/* 按键引脚定义 */
#define WK_UP_GPIO_PORT     GPIOA
#define WK_UP_GPIO_PIN      GPIO_PIN_0
#define WK_UP_CLK_ENABLE()  do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

#define KEY0_GPIO_PORT      GPIOE
#define KEY0_GPIO_PIN       GPIO_PIN_4
#define KEY0_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

#define KEY1_GPIO_PORT      GPIOE
#define KEY1_GPIO_PIN       GPIO_PIN_3
#define KEY1_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

#define KEY2_GPIO_PORT      GPIOE
#define KEY2_GPIO_PIN       GPIO_PIN_2
#define KEY2_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)

/* 按键返回值 */
#define WK_UP_PRESS     1
#define KEY0_PRESS      2
#define KEY1_PRESS      3
#define KEY2_PRESS      4

void key_init(void);
uint8_t key_scan(void);

#endif
