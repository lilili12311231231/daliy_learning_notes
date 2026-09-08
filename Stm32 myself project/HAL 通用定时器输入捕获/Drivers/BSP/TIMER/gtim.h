#ifndef __GTIM_H
#define __GTIM_H
#include "stm32f4xx_hal.h"
/* TIMX ���벶������
 * ��ʾ�����벶��ʹ�ö�ʱ�� TIM5_CH1,Ҳ���� WK_UP ������Ӧ������
 * Ĭ���޸� TIM2~TIM5.
 * ע��: ͨ���޸��⼸���궨��,����֧�� TIM1~TIM8 ����һ����ʱ��,����һ�� IO �������벶��
 * �ر�Ҫע��:Ĭ���õ� PA0,��������������;!���õ����� IO,��Ӧ�жϷ�����Ҳ�ø�!
 */
#define GTIM_TIMX_CAP_CHY_GPIO_PORT GPIOA
#define GTIM_TIMX_CAP_CHY_GPIO_PIN GPIO_PIN_0
#define GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE() \
    do                                      \
    {                                       \
        __HAL_RCC_GPIOA_CLK_ENABLE();       \
    } while (0) /* PA ��ʱ��ʹ�� */
/* AF ��ѡ�� */
#define GTIM_TIMX_CAP_CHY_GPIO_AF GPIO_AF2_TIM5
#define GTIM_TIMX_CAP TIM5
#define GTIM_TIMX_CAP_IRQn TIM5_IRQn
#define GTIM_TIMX_CAP_IRQHandler TIM5_IRQHandler
#define GTIM_TIMX_CAP_CHY TIM_CHANNEL_1 /* ͨ�� Y,1<=Y<=4 */
/* ͨ�� Y ������ȽϼĴ��� */
#define GTIM_TIMX_CAP_CHY_CCRX TIM5->CCR1
extern TIM_HandleTypeDef g_timx_cap_chy_handle;
extern uint8_t  g_timxchy_cap_sta;     /* ����״̬ */
extern uint32_t g_timxchy_cap_val;     /* ����ֵ */

void gtim_timx_cap_chy_init(uint16_t arr, uint16_t psc);

#define GTIM_TIMX_CAP_CHY_CLK_ENABLE() \
    do                                 \
    {                                  \
        __HAL_RCC_TIM5_CLK_ENABLE();   \
    } while (0) /* TIM5 ʱ��ʹ�� */

#endif
