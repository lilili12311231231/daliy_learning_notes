#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

/* 通用定时器 PWM 输出引脚定义 */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOF
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_9
#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF9_TIM14
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

#define GTIM_TIMX_PWM                       TIM14
#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_1
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM14_CLK_ENABLE(); }while(0)
/* 通道1比较寄存器，main.c 里 GTIM_TIMX_PWM_CHY_CCRX = 2 就是设置它 */
#define GTIM_TIMX_PWM_CHY_CCRX              TIM14->CCR1

extern TIM_HandleTypeDef g_timx_pwm_chy_handle;   /* 定时器句柄 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);

#endif
