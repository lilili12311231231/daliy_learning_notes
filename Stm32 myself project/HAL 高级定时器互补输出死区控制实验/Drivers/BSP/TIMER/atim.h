#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"
#define ATIM_TIMX PWMIN_CHY_GPIO_PORT GPIOC 
#define ATIM_TIMX_GPIO_PIN GPIO_PIN_6 
#define ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)             /* GPIOC口时钟使能 */
#define ATIM_TIMX_PWMIN  TIM8 /* 定时器8 */
#define ATIM_TIMX_PWMIN_IRQn TIM8_UP_TIM13_IRQn /* TIM8中断向量 */
#define ATIM_TIMX_PWMIN_IRQHandler TIM8_UP_TIM13_IRQHandler /* TIM8中断处理函数 */
#define ATIM_TIMX_PWMIN_CHY TIM_CHANNEL_1 /* 定时器8通道1 */
#define ATIM_TIMX_PWMIN_CHY_CLK_ENABLE() do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)             /* TIM8时钟使能 */
/* TIM1 / TIM8 有独立的捕获中断服务函数,需要单独定义,对于 TIM2~5 等,则不需要以下定义 */
#define ATIM_TIMX_PWMIN_CC_IRQn TIM8_CC_IRQn /* TIM8通道1中断向量 */
#define ATIM_TIMX_PWMIN_CC_IRQHandler TIM8_CC_IRQHandler /* TIM8通道1中断处理函数 */
#endif 