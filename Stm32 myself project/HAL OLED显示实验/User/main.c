#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
/* 手动声明要用到的 OLED 函数（不包含 oled.h，避免字库重复定义） */
  void oled_init(void);
  void oled_refresh_gram(void);
  void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
  void oled_show_string(uint8_t x, uint8_t y, uint8_t *str, uint8_t size);
  void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);



int main(void)
{
    uint8_t t = 0;
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    usart_init(115200); /* 串口初始化为 115200 */
    delay_init(168);                            /* 延时初始化 */
    led_init();                                 /* 初始化LED */
    oled_init();                                /* 初始化OLED */

    oled_show_string(0, 0, "ALIENTEK", 24);
    oled_show_string(0, 24, "0.96' OLED TEST", 16);
    oled_show_string(0, 52, "ASCII:", 12);
    oled_show_string(64, 52, "CODE:", 12);
    oled_refresh_gram(); /* 更新显示到 OLED */

    t = ' ';

    while(1)
    {
        oled_show_char(36, 52, t, 12, 1); /* 显示 ASCII 字符 */
        oled_show_num(94, 52, t, 3, 12); /* 显示 ASCII 字符的码值 */
        oled_refresh_gram(); /* 更新显示到 OLED */
        t++;
        if (t > '~')
        {
            t = ' ';
        }
        delay_ms(500);
        LED0_TOGGLE(); /* LED0 闪烁 */
    }
}


