#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/CAN/can.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./USMART/usmart.h"


int main(void)
{
    uint8_t key;
    uint8_t i = 0, t = 0;
    uint8_t cnt = 0;
    uint8_t canbuf[8];
    uint8_t rxlen = 0;
    uint8_t res;
    uint8_t mode = 1; /* CAN工作模式 0:普通模式 1:回环模式 */
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                        /* 初始化USART */
    usmart_dev.init(84);                        /* 初始化USMART */
    led_init();                                 /* 初始化LED */
    lcd_init();                                 /* 初始化LCD */
    key_init();                                 /* 初始化按键 */
    can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, CAN_MODE_LOOPBACK);  /* CAN初始化, 环回模式, 波特率500Kbps */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "CAN TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY0:Send KEK_UP:Mode", RED);    /* 显示提示信息 */

    lcd_show_string(30, 150, 200, 16, 16, "Count:", RED);                   /* 显示当前计数值 */
    lcd_show_string(30, 170, 200, 16, 16, "Send Data:", RED);               /* 提示发送的数据 */
    lcd_show_string(30, 230, 200, 16, 16, "Receive Data:", RED);            /* 提示接收到的数据 */

    while(1)
    {
        key = key_scan(0);
        if(key == KEY0_PRES) /* KEY0按下,发送一次数据 */
        {
            for(i = 0; i < 8; i++)
            {
                canbuf[i] = cnt + i; /* 填充发送缓冲区 */
                if(i < 4)
                {
                    lcd_show_xnum(30 + i * 32, 190, canbuf[i], 3, 16, 0x80, BLUE);          /* 显示数据 */
                }
                else
                {
                    lcd_show_xnum(30 + (i - 4) * 32, 210, canbuf[i], 3, 16, 0x80, BLUE);    /* 显示数据 */
                }
            }
            res = can_send_msg(0x12, canbuf, 8);/* ID = 0x12, 发送8个字节 */
            if(res)
            {
                lcd_show_string(30 + 80, 170, 200, 16, 16, "Failed", BLUE); /* 提示发送失败 */
            }
            else
            {
                lcd_show_string(30 + 80, 170, 200, 16, 16, "OK    ", BLUE); /* 提示发送成功 */
            }
        }
        else if(key == WKUP_PRES) /* KEY1按下,切换工作模式 */
        {
            mode = !mode; /* 切换工作模式 */
            /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */
            can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, mode ? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL);

            if(mode == 0) /* 普通模式需要2开发板 */
            {   
                lcd_show_string(30, 110, 200, 16, 16, "Normal Mode  ", RED);
            }
            else /* 回环模式 一个开发板可以测试*/
            {
                lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
            }
        }

        rxlen = can_receive_msg(0x12, canbuf);/* CAN ID = 0x12, 接收数据查询 */
        if(rxlen) /* 接收数据 */
        {
            lcd_fill(30, 270, 130, 310, WHITE); /* 清除之前的显示 */
            for(i = 0; i < rxlen; i++)
            {
                if(i < 4)
                {
                    lcd_show_xnum(30 + i * 32, 250, canbuf[i], 3,16, 0x80, BLUE);/* 显示数据 */
                }
                else
                {
                    lcd_show_xnum(30 + (i - 4) * 32, 270, canbuf[i], 3,16, 0x80, BLUE);/* 显示数据 */
                }
            }
        }

        t++;
        delay_ms(10);

        if(t == 20)
        {
            LED0_TOGGLE(); /*提示系统正在运行*/
            t = 0;
            cnt++;
            lcd_show_xnum(30  + 48, 150, cnt, 3, 16, 0x80, BLUE); /* 显示数值 */
        }
    }
}    