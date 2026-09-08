#include "./BSP/TIMER/gtim.h"

TIM_HandleTypeDef g_timx_pwm_chy_handle;   /* 定时器句柄 */

void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc)
  {
        TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};
        GPIO_InitTypeDef gpio_init_struct = {0};

        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();

        gpio_init_struct.Pin       = GTIM_TIMX_PWM_CHY_GPIO_PIN;
        gpio_init_struct.Mode      = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull      = GPIO_PULLUP;
        gpio_init_struct.Speed     = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = GTIM_TIMX_PWM_CHY_GPIO_AF;
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);

        g_timx_pwm_chy_handle.Instance              = GTIM_TIMX_PWM;
        g_timx_pwm_chy_handle.Init.Prescaler        = psc;
        g_timx_pwm_chy_handle.Init.CounterMode      = TIM_COUNTERMODE_UP;
        g_timx_pwm_chy_handle.Init.Period           = arr;
        HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);

        timx_oc_pwm_chy.OCMode       = TIM_OCMODE_PWM1;
        timx_oc_pwm_chy.Pulse        = 0;                    /* 初值占空比 0 */
        timx_oc_pwm_chy.OCPolarity   = TIM_OCPOLARITY_HIGH;
        timx_oc_pwm_chy.OCFastMode   = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY);

        HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);
    }