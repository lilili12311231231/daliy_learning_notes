#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/btim.h"

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    led_init();                             /* 初始化LED */
    btim_timx_int_init(5000 - 1, 8400 - 1); /* 定时器6初始化, 1s中断一次 */
    while (1)
    {
        LED0_TOGGLE(); /* 翻转LED0 */
        delay_ms(200); /* 延时200ms */
    }
}
