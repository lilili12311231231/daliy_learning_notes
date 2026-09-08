#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h" 
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/gtim.h"

extern TIM_HandleTypeDef g_timx_cnt_chy_handle;/* 定时器2_CH1 实例句柄 */
int main(void)
{
    uint32_t curcnt = 0;/* 计数值 */
    uint32_t oldcnt = 0;/* 上一次计数值 */
    uint8_t key =0;/* 按键状态 */
    uint8_t t = 0;/* 计数器 */
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    usart_init(115200);             /* 初始化串口1，波特率115200 */
    gtim_timx_cnt_init(0);                     /* 初始化定时器2_CH1 */
    gtim_timx_cnt_chy_restart();                     /* 重置定时器2_CH1 */

    while(1)
    {
     key = key_scan();/* 扫描按键 */
     if(key == KEY0_PRESS)//* 按键按下 */
        {
            gtim_timx_cnt_chy_restart();/* 重置定时器2_CH1 */
        }
    
     curcnt = gtim_timx_cnt_chy_get_count();/* 获取定时器2_CH1 计数值 */
     if(curcnt != oldcnt)//* 计数值改变 */
        {
            oldcnt = curcnt;/* 更新上一次计数值 */
            printf("curcnt = %d\n", oldcnt);/* 打印计数值 */
        }
        t++;
        if (t > 20)
        {
            t = 0;/* 重置计数器 */
            LED0_TOGGLE();/* 切换LED0 提示程序运行 */
        }
        delay_ms(10);/* 延时10ms */
    }
}

