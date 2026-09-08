#include "./BSP/TIMER/btim.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_handle; /*定时器6句柄*/
/*定时器中断初始化函数*/

void btim_timx_int_init(uint16_t arr, uint16_t psc)
{
    g_timx_handle.Instance = TIM6;      /*定时器6*/
    g_timx_handle.Init.Prescaler = psc; /*定时器分频*/
    g_timx_handle.Init.Period = arr;    /*定时器计数值*/
    HAL_TIM_Base_Init(&g_timx_handle);  /*定时器6初始化*/

    HAL_TIM_Base_Start_IT(&g_timx_handle); /*使能定时器6中断*/
}
/*定时器基础MSP初始化函数*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) /*定时器6*/
    {
        __HAL_RCC_TIM6_CLK_ENABLE();               /*使能定时器6时钟*/
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 3); /*设置中断优先级*/
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);         /*使能定时器6中断*/
    }
}
/*定时器6中断服务函数*/
void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle); /*定时器6中断处理函数*/
}
/*定时器溢出中断回调函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6) /*定时器6*/
    {
        HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); /*翻转LED0*/
    }
}
