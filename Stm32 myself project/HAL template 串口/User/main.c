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


int main(void)
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    led_init();                                 /* 初始化LED */
    usart_init(115200);                         /* 设置波特率*/
    uint8_t len;
    uint16_t times = 0;
    
    while(1)
    {
        if(g_usart_rx_sta & 0x8000)             /* 接收到了数据? */
        {
            len = g_usart_rx_sta & 0x3fff; /* 得到此次接收到的数据长度 */
            printf("\r\n 您发送的消息为:\r\n");
            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart_rx_buf, len, 1000);  /* 发送接收到的数据 */
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);/* 等待发送结束 */
            printf("\r\n\r\n");
            g_usart_rx_sta = 0;
        }
        else
        {
            times++;
            if (times % 5000 == 0)
            {
                printf("\r\n 正点原子 STM32 开发板 串口实验\r\n");
                printf("正点原子@ALIENTEK\r\n\r\n\r\n");
            }
            if (times % 200 == 0) printf("请输入数据,以回车键结束\r\n");
            
            if (times % 30 == 0) LED0_TOGGLE(); /* 闪烁 LED,提示系统正在运行. */
            
            delay_ms(10);
        }

    }
}

