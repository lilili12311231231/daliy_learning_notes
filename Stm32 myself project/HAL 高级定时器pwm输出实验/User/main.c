#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"
#include "./BSP/TIMER/gtim.h"
/* 主函数 */
int main(void)
{
    uint8_t t = 0;/* 计数器 */
    double ht, ct, f, tpsc;

    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                               /* 初始化串口 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    gtim_timx_pwm_chy_init(10 - 1, 168 - 1);/* 初始化定时器   1Mhz 的计数频率, 100Khz PWM*/
    atim_timx_pwmin_chy_init(); /* 初始化 PWM 输入捕获 */

    GTIM_TIMX_PWM_CHY_CCRX = 2; /* 设置通道 X 输出比较值 */
    while(1)
    {

        delay_ms(10);/* 延时10ms */
        t++;/* 增加计数器 */
        if(t >= 20) /* 每 200ms 输出一次结果,并闪烁 LED0,提示程序运行 */
        {
            if(g_timxchy_pwmin_sta)/* 捕获了一次数据 */
            {
                printf("\r\n");
                printf("PWM PSC : %d\r\n", g_timxchy_pwmin_psc);/* 打印分频系数 */
                printf("PWM Hight:%d\r\n", g_timxchy_pwmin_hval);/*打印高电平脉宽*/ 
                printf("PWM Cycle:%d\r\n", g_timxchy_pwmin_cval);/* 打印周期 */
                /* 得到 PWM 采样时钟周期时间 */
                tpsc = ((double)g_timxchy_pwmin_psc + 1)/72; /* 采样时钟周期时间 */
                ht = (double)g_timxchy_pwmin_hval * tpsc; /* 高电平脉宽时间 */
                ct = (double)g_timxchy_pwmin_cval * tpsc; /* 周期时间 */
                f = (1/ct)*1000000; /* 频率 */
                printf("PWM Hight time:%fus\r\n", ht);/* 打印高电平脉宽时间 */
                printf("PWM Cycle time:%fus\r\n", ct);/* 打印周期时间 */
                printf("PWM Frequency:%fHz\r\n", f);/* 打印频率 */

                atim_timx_pwmin_chy_restart(); /* 重启 PWM 输入捕获 */
            }
            LED0_TOGGLE();/* 切换 LED0 状态 *//* LED0(RED)闪烁 */
            t = 0;/* 重置计数器 */
        }
    }
}

