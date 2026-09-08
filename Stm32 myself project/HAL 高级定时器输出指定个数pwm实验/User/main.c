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
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
    uint8_t key;/* 按键状态 */
    uint16_t t;/* 计数器 */

    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168); 
    usart_init(115200); /* 串口初始化为 115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */

    /* 将 LED1 引脚设置为输入模式, 避免和 PC6 冲突 */
    gpio_init_struct.Pin = LED1_GPIO_PIN;/* LED1 引脚为 PC6 口 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;/* 输入模式 */
    gpio_init_struct.Pull = GPIO_PULLUP;/* 上拉电阻 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 速度 */

    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);/* 初始化 LED1 引脚 */

    atim_timx_npwm_chy_init(10000-1, 8400-1);/* 初始化 TIM8 */
    ATIM_TIMX_NPWM_CHY_CCRX = 5000;/* 设置占空比为 50% */
    atim_timx_npwm_chy_set(5);/* 启动 TIM8 */
    
    while(1)//* 无限循环 */
    {
        key = key_scan();
        if(key == KEY0_PRESS)//* 按键0按下 */
        {
            atim_timx_npwm_chy_set(5);/* 启动 TIM8 */
        }
        t++;/* 计数器增加 */
        delay_ms(10);/* 延时 10ms */
        if(t >= 5)//* 5ms */
        {
            t = 0;/* 重置计数器 */
            LED0_TOGGLE();/* 切换 LED0 状态 */
        }
    }

}

