#ifndef __TPAD_H
#define __TPAD_H
`   #include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/sys/sys.h"
/* TPAD 引脚 及 定时器 定义 硬件引脚定义 */
#define TPAD_GPIO_PORT GPIOA /* GPIOA */
#define TPAD_GPIO_PIN GPIO_PIN_5 /* GPIO_PIN_5 */
#define TPAD_GPIO_AF        GPIO_AF1_TIM2
#define TPAD_GPIO_CLK_ENABLE()  do{__HAL_RCC_GPIOA_CLK_ENABLE();}while(0)
/* PA 口时钟使能 */
/* 定时器定义 */
#define TPAD_TIMX_CAP TIM2
#define TPAD_TIMX_CAP_CHY TIM_CHANNEL_1 /* 通道 Y, 1<= Y <=4 */
#define TPAD_TIMX_CAP_CHY_CLK_ENABLE() do{__HAL_RCC_TIM2_CLK_ENABLE();}while(0) /* 使能 TIM2 时钟 */
#define TPAD_TIMX_CAP_CHY_CCRX      TIM2->CCR1   /* 捕获寄存器 */
/* 参数定义 */
#define TPAD_ARR_MAX_VAL            0xFFFF        /* 自动重装最大值 */
#define TPAD_GATE_VAL               80            /* 触摸阈值 */

/* 全局变量 */
extern uint16_t g_tpad_default_val;              /* 未触摸时的默认值 */
/* 函数声明 */
uint8_t tpad_init(uint16_t psc);
uint8_t tpad_scan(uint8_t mode);
uint16_t tpad_get_maxval(uint8_t n);
#endif
