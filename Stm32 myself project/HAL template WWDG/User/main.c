/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       WWDG实验 主函数
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 淘宝店铺:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/WDG/wdg.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置系统时钟,168Mhz */
    delay_init(168);                    /* 初始化延时函数 */
    led_init();                         /* 初始化LED */
    usart_init(115200);                 /* 初始化串口115200 */
    LED0(0);

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET)
    {
        printf("窗口看门狗复位启动!!\r\n");
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    else
    {
        printf("非看门狗复位\r\n");
    }

    delay_ms(500); /* 延时 500ms 等待系统稳定后启动窗口看门狗,LED0 闪烁表示正常运行 */
    printf("窗口看门狗实验开始!!\r\n");
    wwdg_init(0x7f, 0x5f, WWDG_PRESCALER_8);

    while (1)
    {
        delay_ms(90);
        HAL_WWDG_Refresh(&g_wwdg_handle);
        LED0_TOGGLE();
    }
}
