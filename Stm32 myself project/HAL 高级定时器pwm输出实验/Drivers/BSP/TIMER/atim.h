#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"
#define ATIM_TIMX_PWMIN_CHY_GPIO_PORT GPIOC 
#define ATIM_TIMX_PWMIN_CHY_GPIO_PIN GPIO_PIN_6 
#define ATIM_TIMX_PWMIN_CHY_GPIO_AF  GPIO_AF3_TIM8
#define ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)             /* GPIOC口时钟使能 */
#define ATIM_TIMX_PWMIN  TIM8 /* 定时器8 */
#define ATIM_TIMX_PWMIN_IRQn TIM8_UP_TIM13_IRQn /* TIM8中断向量 */
#define ATIM_TIMX_PWMIN_IRQHandler TIM8_UP_TIM13_IRQHandler /* TIM8中断处理函数 */
#define ATIM_TIMX_PWMIN_CHY TIM_CHANNEL_1 /* 定时器8通道1 */
#define ATIM_TIMX_PWMIN_CHY_CLK_ENABLE() do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)             /* TIM8时钟使能 */
/* TIM1 / TIM8 有独立的捕获中断服务函数,需要单独定义,对于 TIM2~5 等,则不需要以下定义 */
#define ATIM_TIMX_PWMIN_CC_IRQn TIM8_CC_IRQn /* TIM8通道1中断向量 */
#define ATIM_TIMX_PWMIN_CC_IRQHandler TIM8_CC_IRQHandler /* TIM8通道1中断处理函数 */

/* 全局变量声明（extern 只是声明，真正定义在 .c 里） */
extern TIM_HandleTypeDef g_timx_pwmin_chy_handle; /* 定时器句柄 */

extern uint8_t  g_timxchy_pwmin_sta;    /* 捕获状态 */
extern uint32_t g_timxchy_pwmin_psc;    /* 分频系数 */
extern uint32_t g_timxchy_pwmin_hval;   /* 高电平脉宽 */
extern uint32_t g_timxchy_pwmin_cval;   /* 周期 */
/* 函数声明 */
void atim_timx_pwmin_chy_init(void);
void atim_timx_pwmin_chy_restart(void);

#endif 