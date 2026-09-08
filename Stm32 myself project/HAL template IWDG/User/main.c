/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       独立看门狗 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 技术视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司地址:www.alientek.com
 * 淘宝地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/WDG/wdg.h"
#include "./BSP/KEY/key.h"

int main(void)
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    led_init();                                 /* 初始化LED */
    usart_init(115200);                         /* 设置波特率 */
    key_init();                                 /* 初始化按键 */
    delay_ms(100);                              /* 延时100ms再初始化看门狗,LED0的变化可见 */
    iwdg_init(IWDG_PRESCALER_64, 1000);         /* 预分频为64,重载值为2000,超时时间约为4s */
    printf("\r\n 还没喂狗，快喂狗！\r\n");
    LED0(0);
    while(1)
    {
        if (key_scan(1) == WKUP_PRES)           /* 如果KEY_UP按下,则喂狗,支持连按 */
        {
            iwdg_feed();                        /* 喂狗 */
            printf("\r\n 已经喂狗\r\n");
        }
        delay_ms(1000);
    }
}
