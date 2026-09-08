#include "./BSP/TIMER/atim.h"

uint8_t  g_timxchy_pwmin_sta = 0;
uint32_t g_timxchy_pwmin_psc = 0;
uint32_t g_timxchy_pwmin_hval = 0;
uint32_t g_timxchy_pwmin_cval = 0;

TIM_HandleTypeDef g_timx_pwmin_chy_handle;/* 定时器句柄 */

static void atim_timx_pwmin_chy_process(void);   /* 新增这一行（函数声明） */
void atim_timx_pwmin_chy_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_SlaveConfigTypeDef slave_config = {0};
    TIM_IC_InitTypeDef tim_ic_pwmin_chy = {0};

    ATIM_TIMX_PWMIN_CHY_CLK_ENABLE();
    ATIM_TIMX_PWMIN_CHY_GPIO_CLK_ENABLE();


    gpio_init_struct.Pin = ATIM_TIMX_PWMIN_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = ATIM_TIMX_PWMIN_CHY_GPIO_AF;
    HAL_GPIO_Init(ATIM_TIMX_PWMIN_CHY_GPIO_PORT, &gpio_init_struct);

    g_timx_pwmin_chy_handle.Instance = ATIM_TIMX_PWMIN;/* 定时器8 */
    g_timx_pwmin_chy_handle.Init.Prescaler = 0;/* 无分频 */
    g_timx_pwmin_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;/* 上计数模式 */
    g_timx_pwmin_chy_handle.Init.Period = 65535;/* 自动重装载寄存器值 */
    HAL_TIM_IC_Init(&g_timx_pwmin_chy_handle); /* 初始化定时器 */
/* 从模式配置，IT1 触发更新 */
    slave_config.SlaveMode = TIM_SLAVEMODE_RESET; /* 重模式 */
    slave_config.InputTrigger = TIM_TS_TI1FP1; /* 外部触发源 */
    slave_config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* 上升沿触发 */
    slave_config.TriggerFilter = 0; /* 无滤波 */
    HAL_TIM_SlaveConfigSynchro(&g_timx_pwmin_chy_handle, &slave_config); /* 从模式配置 */
/* IC1 捕获：上升沿触发 TI1FP1 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING; /* 上升沿触发 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 直接选择输入 */
    tim_ic_pwmin_chy.ICPrescaler = TIM_ICPSC_DIV1; /* 无分频 */
    tim_ic_pwmin_chy.ICFilter = 0; /* 无滤波 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_1); /* 配置通道1 输入捕获 */
/* IC2 捕获：上升沿触发 TI1FP2 */
    tim_ic_pwmin_chy.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING; /* 下降沿触发 */
    tim_ic_pwmin_chy.ICSelection = TIM_ICSELECTION_INDIRECTTI; /* 间接选择输入 */
    HAL_TIM_IC_ConfigChannel(&g_timx_pwmin_chy_handle, &tim_ic_pwmin_chy, TIM_CHANNEL_2); /* 配置通道2 输入捕获 */
/* 设置中断优先级，抢占优先级 1，子优先级 3 */
    HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_IRQn, 1, 3); /* 设置中断优先级 */
    HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_IRQn); /* 使能中断 */

    /* TIM1/TIM8 有独立的输入捕获中断服务函数 */
    if( ATIM_TIMX_PWMIN == TIM1 || ATIM_TIMX_PWMIN == TIM8)
    {
        /* 设置中断优先级，抢占优先级 1，子优先级 3 */
        HAL_NVIC_SetPriority(ATIM_TIMX_PWMIN_CC_IRQn, 1, 3); /* 设置中断优先级 */
        HAL_NVIC_EnableIRQ(ATIM_TIMX_PWMIN_CC_IRQn); /* 使能中断 */
    }
    __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE); /* 使能通道1/通道2 更新中断 */
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1); /* 使能通道1 输入捕获中断 */
    HAL_TIM_IC_Start_IT(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2); /* 使能通道2 输入捕获中断 */
}
//定时器 TIMX PWM 输入模式 重新启动捕获
void atim_timx_pwmin_chy_restart(void)
{
     sys_intx_disable();/* 禁用中断 */

     g_timxchy_pwmin_sta = 0;/* 重置状态 */
     g_timxchy_pwmin_psc = 0;/* 重置分频系数 */

     __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, 0); /* 重置分频系数 */
     __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0); /* 重置计数器 */

     __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1); /* 重置通道1 输入捕获中断 */
     __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE); /* 重置更新中断 */
     __HAL_TIM_ENABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC2); /* 重置通道2 输入捕获中断 */
    
     ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */

     sys_intx_enable();/* 使能中断 */
}
void ATIM_TIMX_PWMIN_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}

void ATIM_TIMX_PWMIN_CC_IRQHandler(void)
{
    atim_timx_pwmin_chy_process();
}

static void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;/* 状态标志位 */

    if(g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */
        __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0); /* 重置计数器 */
        return;
    }
    if(__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE)) /* 更新中断标志位 */
    {
        /* 清除更新中断标记 */
        __HAL_TIM_CLEAR_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_UPDATE); /* 清除更新中断标志位 */
        /* 没有发生周期捕获中断,且捕获未完成 */
        if(__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1) ==  0) /* 通道1 输入捕获中断标志位 */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0) /* 分频系数为0 */
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65536) /* 分频系数为65536 */
                {
                    g_timxchy_pwmin_psc = 0; /* 重置分频系数 */
                }
                else if (g_timxchy_pwmin_psc > 32767) /* 分频系数大于32767 */
                {
                    g_timxchy_pwmin_psc = 65536; /* 分频系数为65536 */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc; /* 分频系数自增 */
                }
            }
            __HAL_TIM_SET_PRESCALER(&g_timx_pwmin_chy_handle, g_timxchy_pwmin_psc); /* 设置分频系数 */
            __HAL_TIM_SET_COUNTER(&g_timx_pwmin_chy_handle, 0); /* 重置计数器 */
            ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */
            return;
        }
    }
    if(sflag == 0)/* 第一次采集到捕获中断 */
    {
        /* 检测到了第一次周期捕获中断 */
        if(__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1)) /* 通道1 输入捕获中断标志位 */
        {
            sflag = 1;
        }
        ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */
        return; /* 返回 */
    }
    if(g_timxchy_pwmin_sta == 0)
    /* 检测到了周期捕获中断 */
    if(__HAL_TIM_GET_FLAG(&g_timx_pwmin_chy_handle, TIM_FLAG_CC1)) /* 通道1 输入捕获中断标志位 */
    {
        g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_2) + 1; /* 高定平脉宽捕获值  */
        g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&g_timx_pwmin_chy_handle, TIM_CHANNEL_1) + 1; /* 低定平脉宽捕获值,并加1 */
        
        if(g_timxchy_pwmin_hval < g_timxchy_pwmin_cval) /* 高定平脉宽捕获值小于周期捕获值 */
        {
            g_timxchy_pwmin_sta = 1;/* 标记捕获成功 */
            g_timxchy_pwmin_psc = ATIM_TIMX_PWMIN->PSC;/* 获取 PWM 输入分频系数 */
        
            if(g_timxchy_pwmin_psc == 0) /* 分频系数为0 */
            {
                g_timxchy_pwmin_hval++;/* 修正系数为 1, 加 1 */
                g_timxchy_pwmin_cval++;/* 修正系数为 1, 加 1 */
            }
            sflag = 0;
            /* 每次捕获 PWM 输入成功后, 停止捕获,避免频繁中断影响系统正常代码运行 */
            ATIM_TIMX_PWMIN->CR1 &= ~(1 << 0);/* 关闭定时器 TIMX */
            /* 关闭通道 1 捕获中断 */
            __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC1);
            /* 关闭通道 2 捕获中断 */
            __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_CC2);
            /* 关闭更新中断 */
            __HAL_TIM_DISABLE_IT(&g_timx_pwmin_chy_handle, TIM_IT_UPDATE);
            ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */
        }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        
    }
    ATIM_TIMX_PWMIN->SR = 0;/* 重置状态寄存器 */
}