#include "./BSP/WDG/wdg.h"

IWDG_HandleTypeDef g_iwdg_handle;

/*IWDG初始化函数*/
void iwdg_init(uint8_t Prer, uint16_t rlr)
{
    g_iwdg_handle.Instance = IWDG;
    g_iwdg_handle.Init.Prescaler = Prer; /* 设置 IWDG 分频系数 */
    g_iwdg_handle.Init.Reload = rlr; /* 重装载值 */
    HAL_IWDG_Init(&g_iwdg_handle);
}



/*喂狗函数*/
void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&g_iwdg_handle); /* 重装载计数器 */
}
