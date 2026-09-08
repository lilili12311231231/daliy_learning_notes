/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       跑马灯 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
    uint16_t t;/* 计数器 */

    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168); 
    usart_init(115200); /* 串口初始化为 115200 */
    led_init();                                 /* 初始化LED */
    atim_timx_comp_pwm_init(1000-1, 168-1);/* 初始化 TIM8 */
    ATIM_TIMX_COMP_CH1_CCR = 250 - 1;/* 设置占空比为 25% */
    ATIM_TIMX_COMP_CH2_CCR = 500 - 1;/* 设置占空比为 50% */
    ATIM_TIMX_COMP_CH3_CCR = 750 - 1;/* 设置占空比为 75% */
    ATIM_TIMX_COMP_CH4_CCR = 999 - 1;/* 设置占空比为 99% */
    atim_timx_comp_pwm_set(5);/* 启动 TIM8 */


    while(1)//* 无限循环 */
    {
        delay_ms(10);/* 延时 10ms */
        t++;/* 计数器增加 */
       
        if(t >= 20)//* 20ms */
        {
            LED0_TOGGLE();/* 切换 LED0 状态 */
            t = 0;/* 重置计数器 */
        }
    }

}

