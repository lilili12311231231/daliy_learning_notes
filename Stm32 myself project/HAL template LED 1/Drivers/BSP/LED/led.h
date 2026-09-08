#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"
#include "stm32f4xx_hal.h"          // ← 必须引入 HAL 头文件

  /* LED0 引脚定义 */
  #define LED0_GPIO_PORT      GPIOF
  #define LED0_GPIO_PIN       GPIO_PIN_9
  #define LED0_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

  /* LED1 引脚定义 */
  #define LED1_GPIO_PORT      GPIOF
  #define LED1_GPIO_PIN       GPIO_PIN_10
  #define LED1_GPIO_CLK_ENABLE()  do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

  /* LED 操作宏 */
  #define LED0(x)   HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
  #define LED1(x)   HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
void led_init(void);

#endif
