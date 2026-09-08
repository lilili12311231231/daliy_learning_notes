#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/24CXX/24cxx.h"
#include "./BSP/IIC/myiic.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#define TEXT_SIZE (sizeof(g_text_buf))            /* 文本长度 = 缓冲区大小 */

uint8_t g_text_buf[] = {"ALIENTEK IIC TEST"};     /* 要写入 EEPROM 的文本 */
uint8_t datatemp[TEXT_SIZE];                      /* 读回数据的缓冲区 */

int main(void)
{
    uint8_t key;        /* 按键值 */
    uint8_t i = 0;      /* LED 闪烁计数器 */
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为 115200 */
    led_init();                                 /* 初始化LED */
    iic_init();                                 /* 初始化IIC */
    key_init();                                 /* 初始化按键 */
    lcd_init();                                 /* 初始化LCD */
    at24cxx_init();                             /* 初始化AT24CXX */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED); /* 显示字符串 */
    lcd_show_string(30, 70, 200, 16, 16, "IIC TEST", RED); /* 显示字符串 */
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED); /* 显示字符串 */
    lcd_show_string(30, 110, 200, 16, 16, "KEY1:Write KEY0:Read", RED); /* 显示字符串 */

    while (at24cxx_check())/* 检测不到 24c02 */
    {
        lcd_show_string(30, 130, 200, 16, 16, "24C02 Check Failed!", RED); /* 显示字符串 */
        delay_ms(500); /* 延时 500ms */
        lcd_show_string(30, 130, 200, 16, 16, "Please Check! ", RED); /* 显示字符串 */
        delay_ms(500); /* 延时 500ms */
        LED0_TOGGLE(); /* 切换 红灯LED0状态 */
    }
    lcd_show_string(30, 130, 200, 16, 16, "24C02 Ready!   ", RED); /* 显示字符串 */
    while (1)
    {
        key = key_scan(0); 
        if(key == KEY1_PRES) /* KEY1 按下,写入 24C02 */
        {
            lcd_fill(0, 150, 239, 319, WHITE); /* 清除半屏 */
            lcd_show_string(30, 150, 200, 16, 16, "Start Write 24C02", BLUE); /* 显示字符串 */
            at24cxx_write(0, (uint8_t *)g_text_buf, TEXT_SIZE); /* 写入数据 */
            /* 提示传送完成 */
            lcd_show_string(30, 150, 200, 16, 16, "Start Read 24C02.... ", BLUE); /* 显示字符串 */
            at24cxx_read(0, datatemp, TEXT_SIZE); /* 读取数据 */
            /* 显示读到的字符串 */
            lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE); /* 显示字符串 */
        }
         else if (key == KEY0_PRES)                  /* KEY0 按下, 只读不写 */
        {
        lcd_fill(0, 150, 239, 319, WHITE);      /* 清空下方显示区 */
        lcd_show_string(30, 150, 200, 16, 16, "Start Read 24C02....", BLUE);

        memset(datatemp, 0, TEXT_SIZE);         /* 读之前先清空缓冲区 */
        at24cxx_read(0, datatemp, TEXT_SIZE);   /* 从地址 0 读取 TEXT_SIZE 字节 */

        lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE);  /* 显示读到的内容 */
        }
        i++;/* 增加计数器 */
        if(i == 20)
        {
            LED0_TOGGLE(); /* 切换 红灯LED0状态 */
            i = 0;
        }
        delay_ms(10); /* 延时 10ms */
    }
}

