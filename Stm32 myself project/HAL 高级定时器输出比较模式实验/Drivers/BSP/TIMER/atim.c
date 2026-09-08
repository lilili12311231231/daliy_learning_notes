#include "./BSP/TIMER/atim.h"
TIM_HandleTypeDef g_timx_comp_pwm_handle;   /* ① 定义全局定时器句柄 */
void atim_timx_comp_pwm_init(uint16_t psc, uint16_t arr)
{
   
    TIM_OC_InitTypeDef timx_oc_comp_pwm;/* TIM 输出比较初始化结构体 */
    ATIM_TIMX_COMP_GPIO_CLK_ENABLE();/* PC6 口时钟使能 */
    ATIM_TIMX_COMP_CLK_ENABLE();/* TIM8 时钟使能 */

    g_timx_comp_pwm_handle.Instance = ATIM_TIMX_COMP;/* TIM8 实例 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc;/* 时钟分频数 */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;/* 上计计数模式 */
    g_timx_comp_pwm_handle.Init.Period = arr;/* 自动重装值 */
    g_timx_comp_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;/* 时钟分频数 */
    g_timx_comp_pwm_handle.Init.RepetitionCounter = 0;/* 重复计数器 */
    g_timx_comp_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;/* 自动重装预加载使能 */

    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);/* 启动定时器 */

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;/* 输出比较模式, 切换输出状态 模式 */
    timx_oc_comp_pwm.Pulse = 250-1;/* 占空比 50%    */
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;/* 高电平有效 */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_1);/* 配置通道1 */
     __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);/* 使能通道1 预加载 */
   
    timx_oc_comp_pwm.Pulse = 500;/* 占空比 50% + 50% = 100%    */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_2);/* 配置通道2 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);/* 使能通道2 预加载 */
      
    timx_oc_comp_pwm.Pulse = 750;/* 占空比 75% + 25% = 100%    */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_3);/* 配置通道3 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);/* 使能通道3 预加载 */
   
    timx_oc_comp_pwm.Pulse = 1000;/* 占空比 100% + 0%     */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_4);/* 配置通道4 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);/* 使能通道4 预加载 */
    
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);/* 启动定时器 */
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);/* 启动定时器 */
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);/* 启动定时器 */
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);/* 启动定时器 */   

}
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == ATIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
        ATIM_TIMX_COMP_GPIO_CLK_ENABLE();/* PC6 口时钟使能 */
        ATIM_TIMX_COMP_GPIO_CH1_CLK_ENABLE();/* PC6 口时钟使能 */
        ATIM_TIMX_COMP_GPIO_CH2_CLK_ENABLE();/* PC7 口时钟使能 */
        ATIM_TIMX_COMP_GPIO_CH3_CLK_ENABLE();/* PC8 口时钟使能 */
        ATIM_TIMX_COMP_GPIO_CH4_CLK_ENABLE();/* PC9 口时钟使能 */

        gpio_init_struct.Pin = ATIM_TIMX_COMP_GPIO_CH1_PIN;/* PC6 口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 推挽输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;/* 无下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 高速 */
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;/* 通道1 复用功能 */
        HAL_GPIO_Init(ATIM_TIMX_COMP_GPIO_CH1_PORT, &gpio_init_struct);/* 初始化 GPIOC */
        
        gpio_init_struct.Pin = ATIM_TIMX_COMP_GPIO_CH2_PIN;/* PC7 口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 推挽输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;/* 无下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 高速 */
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;/* 通道2 复用功能 */
        HAL_GPIO_Init(ATIM_TIMX_COMP_GPIO_CH2_PORT, &gpio_init_struct);/* 初始化 GPIOC */

        gpio_init_struct.Pin = ATIM_TIMX_COMP_GPIO_CH3_PIN;/* PC8 口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 推挽输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;/* 无下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 高速 */
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;/* 通道3 复用功能 */
        HAL_GPIO_Init(ATIM_TIMX_COMP_GPIO_CH3_PORT, &gpio_init_struct);/* 初始化 GPIOC */
        
        gpio_init_struct.Pin = ATIM_TIMX_COMP_GPIO_CH4_PIN;/* PC9 口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 推挽输出 */
        gpio_init_struct.Pull = GPIO_NOPULL;/* 无下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 高速 */
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;/* 通道4 复用功能 */
        HAL_GPIO_Init(ATIM_TIMX_COMP_GPIO_CH4_PORT, &gpio_init_struct);/* 初始化 GPIOC */
        
    }

}