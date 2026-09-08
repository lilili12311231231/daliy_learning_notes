#include <stdio.h>
#include "./BSP/TPAD/tpad.h"
#include "./SYSTEM/delay/delay.h"
TIM_HandleTypeDef g_timx_cap_chy_handler;/* TIM5 处理结构体 */
TIM_IC_InitTypeDef g_timx_ic_cap_chy_handler;/* TIM5 IC 处理结构体 */
uint16_t g_tpad_default_val;   /* 未触摸时的默认捕获值 */
/**
* @brief 触摸按键输入捕获设置
* @param arr ：自动重装值
* @param psc ：时钟预分频数
* @retval 无
*/
static void tpad_timx_cap_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
    TPAD_GPIO_CLK_ENABLE();/* 使能 GPIOA 时钟 */
    TPAD_TIMX_CAP_CHY_CLK_ENABLE();/* 使能 TIM2 时钟 */

    gpio_init_struct.Pin = TPAD_GPIO_PIN;/* GPIO_PIN_5 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;/* 无上拉下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 速度高 */
    gpio_init_struct.Alternate = TPAD_GPIO_AF;/* 复用功能 */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct);/* 初始化 GPIOA */

    g_timx_cap_chy_handler.Instance = TPAD_TIMX_CAP;/* TIM5 */
    g_timx_cap_chy_handler.Init.Prescaler = psc;/* 预分频数 */
    g_timx_cap_chy_handler.Init.CounterMode = TIM_COUNTERMODE_UP;/* 上计计数模式 */
    g_timx_cap_chy_handler.Init.Period = arr;/* 自动重装值 */
    g_timx_cap_chy_handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;/* 时钟分频 */
    HAL_TIM_IC_Init(&g_timx_cap_chy_handler);

    g_timx_ic_cap_chy_handler.ICPolarity = TIM_ICPOLARITY_RISING;/* 上升边沿 */
    g_timx_ic_cap_chy_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;/* 直接选择 */
    g_timx_ic_cap_chy_handler.ICPrescaler = TIM_ICPSC_DIV1;/* 无分频 */
    g_timx_ic_cap_chy_handler.ICFilter = 0;/* 无滤波 */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handler, &g_timx_ic_cap_chy_handler,TPAD_TIMX_CAP_CHY);

    HAL_TIM_IC_Start(&g_timx_cap_chy_handler, TPAD_TIMX_CAP_CHY);
}
/**
* @brief 复位 TPAD
* @note 我们将 TPAD 按键看做是一个电容, 当手指按下/不按下时容值有变化
* 该函数将 GPIO 设置成推挽输出, 然后输出 0, 进行放电, 然后再设置
336
STM32F407 开发指南
正点原子探索者 STM32F407 开发板教程
* GPIO 为浮空输入, 等待外部大电阻慢慢充电
* @param 无
* @retval 无
*/
static void tpad_reset(void)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO 初始化结构体 */
    gpio_init_struct.Pin = TPAD_GPIO_PIN;/* GPIO_PIN_5 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;/* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;/* 下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 速度高 */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct);/* 初始化 GPIOA */

    /* TPAD 引脚输出 0, 放电 */
    HAL_GPIO_WritePin(TPAD_GPIO_PORT, TPAD_GPIO_PIN, GPIO_PIN_RESET);
    delay_ms(10); /* 等待 10ms */
    /* 清除中断标志位 */
    g_timx_cap_chy_handler.Instance->SR = 0;/* 清除中断标志位 */
    g_timx_cap_chy_handler.Instance->CNT = 0;/* 清除计数器 */

    gpio_init_struct.Pin = TPAD_GPIO_PIN;/* GPIO_PIN_5 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;/* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;/* 无上拉下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 速度高 */
    gpio_init_struct.Alternate = TPAD_GPIO_AF;/* 复用功能 */
    HAL_GPIO_Init(TPAD_GPIO_PORT, &gpio_init_struct);/* 初始化 GPIOA */
}
/**
* @brief 得到定时器捕获值
* @note 如果超时, 则直接返回定时器的计数值
* 我们定义超时时间为: TPAD_ARR_MAX_VAL - 500
* @param 无
* @retval 捕获值/计数值（超时的情况下返回）
*/
static uint16_t tpad_get_val(void)
{
    uint32_t flag = (TPAD_TIMX_CAP_CHY == TIM_CHANNEL_1) ? TIM_FLAG_CC1 :
                  (TPAD_TIMX_CAP_CHY == TIM_CHANNEL_2) ? TIM_FLAG_CC2 :
                  (TPAD_TIMX_CAP_CHY == TIM_CHANNEL_3) ? TIM_FLAG_CC3 :
                  TIM_FLAG_CC4;
 
    tpad_reset();
    while(__HAL_TIM_GET_FLAG(&g_timx_cap_chy_handler, flag) == RESET);
    {
        if(g_timx_cap_chy_handler.Instance->CNT > TPAD_ARR_MAX_VAL - 500)
        {
            return g_timx_cap_chy_handler.Instance->CNT;
        }
    }
    return TPAD_TIMX_CAP_CHY_CCRX; /* 返回捕获值 */
}
/**
* @brief  多次采样取最大值
* @param  n: 采样次数
* @retval 最大值
*/
static uint16_t tpad_get_maxval(uint8_t n)
{
    uint16_t res = 0;
    uint8_t i;
    for(i = 0; i < n; i++)
    {
        uint16_t rval= tpad_get_val();
        if(rval > res)
        {
            res = rval;
        }
    }
    return res;
}

/**
* @brief 初始化触摸按键
* @param psc : 分频系数(值越小, 越灵敏, 最小值为: 1)
* @retval 0, 初始化成功; 1, 初始化失败;
*/
uint8_t tpad_init(uint16_t psc)
{
    uint16_t buf[10]; /* 缓冲区 */
    uint8_t j, i; /* 循环变量 */
    uint16_t temp; /* 临时变量 */
    /* 以 Ft / (psc - 1)Mhz 的频率计数 @Ft = 定时器工作频率*/
    tpad_timx_cap_init(TPAD_ARR_MAX_VAL, psc - 1); /* 初始化定时器 */

    for(i = 0; i < 10; i++)
    {
        buf[i] = tpad_get_val();/* 获取捕获值 */
        delay_ms(10); /* 等待 10ms */
    }
    for(i = 0; i < 9; i++) /* 对比 9 个值 */
    {
        for(j = i + 1; j < 10; j++) 
        {
            if(buf[i] > buf[j]) /* 对比 */
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    temp = 0;
    for(i = 2; i < 8; i++) /* 对比 6 个值 */
    {
        temp += buf[i];/* 对比 冒泡排序（从小到大）*/
    }
    g_tpad_default_val = temp / 6;   /* 计算平均值：去掉最大2个和最小2个，取中间6个*/
    printf("g_tpad_default_val = %d\r\n", g_tpad_default_val);

    if(g_tpad_default_val > TPAD_ARR_MAX_VAL / 2) /* 对比 */
    {
        return 1; /* 初始化失败 */
    }
        return 0; /* 初始化遇到超过 TPAD_ARR_MAX_VAL/2 的数值,不正常! */
    
}
/**
* @brief 扫描触摸按键
* @param mode ：扫描模式
* @arg 0, 不支持连续触发(按下一次必须松开才能按下一次);
* @arg 1, 支持连续触发(可以一直按下)
* @retval 0, 没有按下; 1, 有按下;
*/
uint8_t tpad_scan(uint8_t mode)
{
    static uint8_t keyen = 0; /* 按键状态 */
    uint8_t res = 0; /* 结果 */
    uint8_t sample = 3; /* 采样次数为 3 次 */
    uint16_t rval; /* 捕获值 */
    if(mode)
    {
        sample = 6; /* 支持连续触发, 采样次数为 6 次 */
        keyen = 0; /* 按键状态重置为 0, 以便检测到按键按下 */
    }
    rval = tpad_get_maxval(sample);/* 获取最大值 */
    if(rval > (g_tpad_default_val + TPAD_GATE_VAL)) /* 对比 */
    {/* 大于 tpad_default_val+TPAD_GATE_VAL,有效 */
        res = 1; /* 有按下 */
    }
    keyen = 3; /* 更新按键状态 */
    if (keyen)keyen--;
    return res;
}
