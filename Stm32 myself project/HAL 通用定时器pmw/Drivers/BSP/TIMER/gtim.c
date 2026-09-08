#include "./BSP/TIMER/gtim.h"

/**
 * @brief  初始化定时器输入捕获通道CHY
 * @param  arr: 定时器自动重装载值
 * @param  psc: 定时器分频系数
 * @retval None
 */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0}; // 定时器输入捕获配置结构体

    g_timx_cap_chy_handle.Instance = GTIMx_CAP_CHY;              // 定时器实例
    g_timx_cap_chy_handle.Init.Prescaler = psc;                  // 定时器分频系数
    g_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; // 定时器计数模式
    g_timx_cap_chy_handle.Init.Period = arr;                     // 定时器自动重装载值
    HAL_TIM_IC_Init(&g_timx_cap_chy_handle);                     // 初始化定时器输入捕获

    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;     // 输入捕获极性
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; // 输入捕获选择
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;           // 输入捕获分频系数
    timx_ic_cap_chy.ICFilter = 0;                           // 输入捕获滤波器

    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, TIM_CHANNEL_1); // 配置定时器输入捕获通道1
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_UPDATE);                        // 使能定时器更新中断
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                    // 启动定时器输入捕获通道1中断
}
