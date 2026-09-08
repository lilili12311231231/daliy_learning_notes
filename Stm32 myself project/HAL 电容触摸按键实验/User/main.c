#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TPAD/tpad.h"



int main(void)
{
    uint8_t t = 0;
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                        /* 初始化串口 */
    led_init();                                 /* 初始化LED */
    tpad_init(8);                                /* 初始化触摸按键 */
    while(1)
    {
        if(tpad_scan(0))/* 成功捕获到了一次上升沿(此函数执行时间至少 15ms) */
        {
            LED1_TOGGLE(); /* LED1 切换状态 */
        }
        t++;
        if(t == 15)
        {
            t = 0;
            LED0_TOGGLE(); /* LED0 切换状态 */  
        }
        delay_ms(10);
    }
}

/*tpad_init(8)函数执行之后，就开始触摸按键的扫描。当有触摸时，对
LED1 状态取反，而 LED0 则有规律的间隔取反，提示程序正在运行。*/
