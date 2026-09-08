  #ifndef __ATIM_H__
  #define __ATIM_H__

  #include "./SYSTEM/sys/sys.h"

  /* TIM8 时钟使能 */
  #define ATIM_TIMX_COMP_CLK_ENABLE()             do{ __HAL_RCC_TIM8_CLK_ENABLE();  }while(0)
  /* GPIOC 时钟使能 */
  #define ATIM_TIMX_COMP_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

  /* 每个通道单独的 GPIO 时钟使能(PC6/7/8/9 都在 GPIOC,内容相同) */
  #define ATIM_TIMX_COMP_GPIO_CH1_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
  #define ATIM_TIMX_COMP_GPIO_CH2_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
  #define ATIM_TIMX_COMP_GPIO_CH3_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
  #define ATIM_TIMX_COMP_GPIO_CH4_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)

  /* 定时器实例 */
  #define ATIM_TIMX_COMP                          TIM8

  /* 各通道 GPIO 端口 */
  #define ATIM_TIMX_COMP_GPIO_CH1_PORT            GPIOC
  #define ATIM_TIMX_COMP_GPIO_CH2_PORT            GPIOC
  #define ATIM_TIMX_COMP_GPIO_CH3_PORT            GPIOC
  #define ATIM_TIMX_COMP_GPIO_CH4_PORT            GPIOC

  /* 各通道 GPIO 引脚 */
  #define ATIM_TIMX_COMP_GPIO_CH1_PIN             GPIO_PIN_6   /* PC6 = TIM8_CH1 */
  #define ATIM_TIMX_COMP_GPIO_CH2_PIN             GPIO_PIN_7   /* PC7 = TIM8_CH2 */
  #define ATIM_TIMX_COMP_GPIO_CH3_PIN             GPIO_PIN_8   /* PC8 = TIM8_CH3 */
  #define ATIM_TIMX_COMP_GPIO_CH4_PIN             GPIO_PIN_9   /* PC9 = TIM8_CH4 */

  /* 复用功能 */
  #define ATIM_TIMX_COMP_GPIO_AF                  GPIO_AF3_TIM8

  /* 各通道比较寄存器(直接写它 = 设置比较值) */
  #define ATIM_TIMX_COMP_CH1_CCR                  (ATIM_TIMX_COMP->CCR1)
  #define ATIM_TIMX_COMP_CH2_CCR                  (ATIM_TIMX_COMP->CCR2)
  #define ATIM_TIMX_COMP_CH3_CCR                  (ATIM_TIMX_COMP->CCR3)
  #define ATIM_TIMX_COMP_CH4_CCR                  (ATIM_TIMX_COMP->CCR4)

  /* 函数声明 */
  void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc);   /* 输出比较初始化 */
  void atim_timx_comp_pwm_set(uint8_t num);                   /* 启动输出通道 */

  #endif
