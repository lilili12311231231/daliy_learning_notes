/**
 * @brief 通用定时器 TIMX 通道 Y PWM 输出 初始化函数（使用 PWM 模式 1）
 * @note
 * 通用定时器的时钟来自 APB1,当 D2PPRE1≥2 分频的时候
 * 通用定时器的时钟为 APB1 时钟的 2 倍, 而 APB1 为 42M, 所以定时器时钟 = 84Mhz
 * 定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 * Ft=定时器工作频率,单位:Mhz
 * @param arr: 自动重装值。
 * @param psc: 时钟预分频数
 * @retval 无
 */
#include "./BSP/TIMER/gtim.h"
#include "stm32f4xx_hal.h"

extern uint8_t g_timxchy_cap_sta;
extern uint32_t g_timxchy_cap_val;
TIM_HandleTypeDef g_timx_cap_chy_handle;  // 定时器句柄
uint8_t g_timxchy_cap_sta = 0;            // 捕获状态
uint32_t g_timxchy_cap_val = 0;           // 捕获值

/**
 * @brief 通用定时器 TIMX 通道 Y 输入捕获 初始化函数
 * @note
 * 通用定时器的时钟来自 APB1,当 PPRE1 ≥ 2 分频的时候
 * 通用定时器的时钟为 APB1 时钟的 2 倍, 而 APB1 为 42M, 所以定时器时钟 = 84Mhz
 * 定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 * Ft=定时器工作频率,单位:Mhz
 *
 * @param arr: 自动重装值
 * @param psc: 时钟预分频数
 * @retval 无
 */
void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
    g_timx_cap_chy_handle.Instance = GTIM_TIMX_CAP;              /* 定时器 5 */
    g_timx_cap_chy_handle.Init.Prescaler = psc;                  /* 定时器分频 */
    g_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; /* 递增计数模式 */
    g_timx_cap_chy_handle.Init.Period = arr;                     /* 自动重装载值 */

    HAL_TIM_IC_Init(&g_timx_cap_chy_handle); /* 初始化定时器 5 */

    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;     /* 上升沿捕获 */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 映射到 TI1 上 */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;           /* 配置输入分频，不分频 */
    timx_ic_cap_chy.ICFilter = 0;
    /* 配置输入滤波器，不滤波 */
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy, GTIM_TIMX_CAP_CHY); /* 配置 TIM5 通道 1 */
    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_UPDATE);                            /* 使能更新中断 */
                                                                                           /* 使能通道输入以及使能捕获中断*/
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);
}
/**
 * @brief 通用定时器输入捕获初始化接口
 * HAL 库调用的接口，用于配置不同的输入捕获
 * @param htim:定时器句柄
 * @note 此函数会被 HAL_TIM_IC_Init()调用
 * @retval 无
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP) /*输入通道捕获*/
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_CAP_CHY_CLK_ENABLE();                    /* 使能 TIMx 时钟 */
        GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE();               /* 开启捕获 IO 的时钟 */
        gpio_init_struct.Pin = GTIM_TIMX_CAP_CHY_GPIO_PIN; /* 输入捕获的 GPIO 口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;           /* 复用推挽 */
        gpio_init_struct.Pull = GPIO_PULLDOWN;             /* 下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;     /* 高速 */
        /* 复用为捕获 TIM5 的通道 1 */
        gpio_init_struct.Alternate = GTIM_TIMX_CAP_CHY_GPIO_AF;
        HAL_GPIO_Init(GTIM_TIMX_CAP_CHY_GPIO_PORT, &gpio_init_struct);
        HAL_NVIC_SetPriority(GTIM_TIMX_CAP_IRQn, 1, 3); /* 抢占 1，子优先级 3 */
        HAL_NVIC_EnableIRQ(GTIM_TIMX_CAP_IRQn);         /* 开启 ITMx 中断 */
    }
}
/**
 * @brief 定时器输入捕获中断处理回调函数
 * @param htim:定时器句柄指针
 * @note 该函数在 HAL_TIM_IRQHandler 中会被调用
 * @retval 无
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if ((g_timxchy_cap_sta & 0X80) == 0) /* 还没成功捕获 */
    {
        if (g_timxchy_cap_sta & 0X40) /* 捕获到一个下降沿 */
        {
            g_timxchy_cap_sta |= 0X80; /* 标记成功捕获到一次高电平脉宽 */
            g_timxchy_cap_val =
                HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                  /* 获取当前的捕获值 */
            TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                      /* 一定要先清除原来的设置 */
            TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_RISING); /* 配置 TIM5 通道 1 上升沿捕获 */
        }
        else /* 还未开始,第一次捕获上升沿 */
        {
            g_timxchy_cap_sta = 0; /* 清空 */
            g_timxchy_cap_val = 0;
            g_timxchy_cap_sta |= 0X40;                                                                   /* 标记捕获到了上升沿 */
            __HAL_TIM_SET_COUNTER(&g_timx_cap_chy_handle, 0);                                           /* 计数器清零 */
            TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY);                       /* 一定要先清除原来的设置！！ */
            TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_FALLING); /* 定时器 5 通道 1 设置为下降沿捕获 */
        }
    }
}
/**
 * @brief 定时器更新中断回调函数
 * @param htim:定时器句柄指针
 * @note 此函数会被定时器中断函数共同调用的
 * @retval 无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0) /* 还没成功捕获 */
        {
            if (g_timxchy_cap_sta & 0X40) /* 已经捕获到高电平了 */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F) /* 高电平太长了 */
                {
                    TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,
                                              GTIM_TIMX_CAP_CHY); /* 一定要先清除原来的设置 */
                    /* 配置 TIM5 通道 1 上升沿捕获 */
                    TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle,
                                            GTIM_TIMX_CAP_CHY, TIM_ICPOLARITY_RISING);
                    g_timxchy_cap_sta |= 0X80; /* 标记成功捕获了一次 */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else /* 累计定时器溢出次数 */
                {
                    g_timxchy_cap_sta++;
                }
            }
        }
    }
}
