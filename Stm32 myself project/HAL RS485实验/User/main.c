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
#include "./BSP/RS485/rs485.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./USMART/usmart.h"




int main(void)
{
    uint8_t key; /* 键值 */
    uint8_t i = 0, t = 0; /* 循环变量 */
    uint8_t cnt = 0; /* 接收数据长度 */
    uint8_t rs485buf[5]; /* 接收缓冲区 */
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                          /* 初始化USART */
    led_init();                                 /* 初始化LED */
    lcd_init();                                 /* 初始化LCD */
    key_init();                                 /* 初始化按键 */
    rs485_init(9600);                        /* 初始化RS485 */
    usmart_dev.init(84);   // USMART 调试组件初始化

    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "RS485 TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Send", RED);    /* 显示提示信息 */

    lcd_show_string(30, 130, 200, 16, 16, "Count:", RED);       /* 显示当前计数值 */
    lcd_show_string(30, 150, 200, 16, 16, "Send Data:", RED);   /* 提示发送的数据 */
    lcd_show_string(30, 190, 200, 16, 16, "Receive Data:", RED);/* 提示接收到的数据 */
    
    while(1)
    {
       key = key_scan(0);
       if(key == KEY0_PRES) /* 按键0按下 发送数据 */
       {
            for(i = 0; i < 5; i++)
            {
                rs485buf[i] = cnt + i; /*填充发送数据区*/
                lcd_show_xnum(30 + i * 32, 170, rs485buf[i], 3, 16, 0X80, BLUE);    /* 显示数据 */
            }
            rs485_send_data(rs485buf, 5); /* 发送5byte节数据 */
       }
       rs485_receive_data(rs485buf, &key); /* 接收5byte节数据 */
       if(key)
       {
            if(key > 5)key = 5; 
            for(i = 0; i < key; i++)
            {
                lcd_show_xnum(30 + i * 32, 210, rs485buf[i], 3, 16, 0X80, BLUE);    /* 显示数据 */
            }
       }
       t++;
       delay_ms(10);
       if(t == 20)
       {
            LED0_TOGGLE(); /* LED0 闪烁 */
            t = 0;
            cnt++;
            lcd_show_xnum(30 + 48, 130, cnt, 3, 16, 0X80, BLUE);                    /* 显示数据 */
       }
    }
}

