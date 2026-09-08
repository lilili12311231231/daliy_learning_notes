#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"
#define ATIM_TIMX_NPWM_CHY_GPIO_PORT GPIOC // 通道 Y 对应 GPIO 口
#define ATIM_TIMX_NPWM_CHY_GPIO_PIN GPIO_PIN_6 // 通道 Y 对应 GPIO 口
#define ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE() do{__HAL_RCC_GPIOC_CLK_ENABLE();}while(0) /* GPIOC 时钟使能 */
#define ATIM_TIMX_NPWM_CHY_GPIO_AF GPIO_AF3_TIM8 /* AF 功能选择 */
#define ATIM_TIMX_NPWM TIM8 /* 定时器 */
#define ATIM_TIMX_NPWM_IRQn TIM8_UP_TIM13_IRQn /* 中断请求号, TIM13 中断请求号为 TIM8_UP 中断请求号 */
#define ATIM_TIMX_NPWM_IRQHandler TIM8_UP_TIM13_IRQHandler /* 中断服务函数, TIM13 中断服务函数为 TIM8_UP 中断服务函数 */
#define ATIM_TIMX_NPWM_CHY TIM_CHANNEL_1 /* 通道 Y, 1<= Y <=2 */
#define ATIM_TIMX_NPWM_CHY_CLK_ENABLE() \
        do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0) /* TIM8 时钟使能 */
#define ATIM_TIMX_NPWM_CHY_CCRX   TIM8->CCR1   /* 通道1比较寄存器，写它=设置占空比 */

void atim_timx_npwm_chy_init(uint16_t psc, uint16_t arr);/* 初始化 TIMX 输出指定个数的 PWM 信号 */
void atim_timx_npwm_chy_set(uint32_t npwm);/* 设置 TIMX 输出指定个数的 PWM 信号占空比 */


#endif
