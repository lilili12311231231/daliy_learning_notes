#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/gtim.h"

int main(void)
{
    uint32_t temp = 0;
    uint8_t t = 0;
    HAL_Init();                             /* 初始化 HAL 库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟, 168Mhz */
    delay_init(168);                        /* 初始化延时函数 */
    usart_init(115200);                     /* 初始化串口 */
    led_init();                             /* 初始化 LED */
    key_init();                             /* 初始化按键 */
    gtim_timx_cap_chy_init(0XFFFF, 84 - 1); /* 以 1Mhz 的频率计数 捕获 */
    while (1)
    {
        if (g_timxchy_cap_sta & 0X80) /* 成功捕获到一次高电平脉宽 */
        {
            temp = g_timxchy_cap_sta & 0X3F; /* 获取溢出次数 */
            temp *= 65536;                   /* 加上溢出时间 */
            temp += g_timxchy_cap_val;       /* 加上捕获值 */
            printf("HIGH:%d us\r\n", temp);  /* 打印高电平脉宽 */
            g_timxchy_cap_sta = 0;           /* 清除状态 */
        }
        t++;         /* 每 100ms 翻转一次 LED */
        if (t >= 20) /* 100ms 翻转一次 LED */
        {
            t = 0;
            LED0_TOGGLE(); /* 翻转 LED0  LED0 闪烁 ,提示系统运行*/
        }
        delay_ms(10); /* 延时 10ms */
    }
}
