#include "./BSP/TIMER/atim.h"

TIM_HandleTypeDef g_timx_npwm_chy_handle;/* 定时器8_CH1 实例句柄 */


/**
* @brief 高级定时器 TIMX 通道 Y 输出指定个数 PWM 初始化函数
* @note
* 高级定时器的时钟来自 APB2, 而 PCLK2 = 168Mhz, 我们设置 PPRE2 不分频, 因此
* 高级定时器时钟 = 168Mhz
* 定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
* Ft=定时器工作频率,单位:Mhz
* @param arr: 自动重装值
* @param psc: 时钟预分频数
* @retval 无
*/
void atim_timx_npwm_chy_init(uint16_t psc, uint16_t arr)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
    TIM_OC_InitTypeDef timx_oc_npwm_chy;/* TIM 输出比较初始化结构体 */
    ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE();/* PC6 口时钟使能 */
    ATIM_TIMX_NPWM_CHY_CLK_ENABLE();/* TIM8 时钟使能 */

    g_timx_npwm_chy_handle.Instance = ATIM_TIMX_NPWM;/* TIM8 实例 */
    g_timx_npwm_chy_handle.Init.Prescaler = psc;/* 时钟分频数 */
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;/* 上计计数模式 */
    g_timx_npwm_chy_handle.Init.Period = arr;/* 自动重装值 */
    g_timx_npwm_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;/* 时钟分频数 */
    g_timx_npwm_chy_handle.Init.RepetitionCounter = 0;/* 重复计数器 */
    g_timx_npwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;/* 自动重装预加载使能 */

    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);/* 启动定时器 */

    gpio_init_struct.Pin = ATIM_TIMX_NPWM_CHY_GPIO_PIN;/* PC6 口 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;/* 上拉电阻 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 速度 */
    gpio_init_struct.Alternate = ATIM_TIMX_NPWM_CHY_GPIO_AF;/* 复用功能 */
    HAL_GPIO_Init(ATIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);/* 初始化 GPIO */

    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;/* PWM1 模式 */
    timx_oc_npwm_chy.Pulse = arr / 2;/* 占空比 */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;/* 高电平有效 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &timx_oc_npwm_chy, ATIM_TIMX_NPWM_CHY);/* 配置通道Y */
    HAL_NVIC_SetPriority(ATIM_TIMX_NPWM_IRQn, 1, 3);/* 设置中断优先级 */
    HAL_NVIC_EnableIRQ(ATIM_TIMX_NPWM_IRQn);/* 使能中断 */
    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);/* 使能更新中断 */
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, ATIM_TIMX_NPWM_CHY);/* 启动通道Y */

}
/* g_npwm_remain 表示当前还剩下多少个脉冲要发送
* 每次最多发送 256 个脉冲
*/
static uint32_t g_npwm_remain = 0;/* 剩余脉冲数 */
/**
* @brief 高级定时器 TIMX NPWM 设置 PWM 个数
* @param rcr: PWM 的个数, 1~2^32 次方个
* @retval 无
*/
void atim_timx_npwm_chy_set(uint32_t npwm) /* 设置 PWM 个数 */
{
    if(npwm == 0)return;/* 0 表示不发送 */
    g_npwm_remain = npwm;/* 剩余脉冲数 */
    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);/* 生成更新事件 */
    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);/* 使能定时器 */

}
/**
* @brief 定时器中断服务函数
* @param 无
* @retval 无
*/
void ATIM_TIMX_NPWM_IRQHandler(void)
{
    uint16_t npwm = 0;/* PWM 个数 */
    if(__HAL_TIM_GET_FLAG(&g_timx_npwm_chy_handle, TIM_FLAG_UPDATE) !=RESET)//更新中断标志位
    {
        if(g_npwm_remain >= 256)/* 剩余脉冲数大于等于 256 个, 则发送 256 个脉冲 */
        {
            g_npwm_remain -= 256;/* 剩余脉冲数减 256 */
            npwm = 256;/* 发送 256 个脉冲 */
        }
        else if(g_npwm_remain % 256)/* 剩余脉冲数小于 256 个, 则发送剩余脉冲数 */
        {
            npwm = g_npwm_remain % 256;/* 发送剩余脉冲数 */
            g_npwm_remain = 0;/* 剩余脉冲数清零 */
        }
        if(npwm)//* 发送脉冲 */
        {
            ATIM_TIMX_NPWM->RCR = npwm -1;/* 发送脉冲 */
            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);/* 生成更新事件 */
            __HAL_TIM_DISABLE(&g_timx_npwm_chy_handle);/* 禁用定时器 */
        }
        else
        {
            ATIM_TIMX_NPWM->CR1 = 0;/* 禁用通道Y */
        }
        __HAL_TIM_CLEAR_FLAG(&g_timx_npwm_chy_handle, TIM_FLAG_UPDATE);/* 清除更新标志位 */
    }
}
