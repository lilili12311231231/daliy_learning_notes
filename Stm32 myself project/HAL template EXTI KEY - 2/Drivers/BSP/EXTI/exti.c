#include "./BSP/EXTI/exti.h"
#include "./SYSTEM/delay/delay.h"

void exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    gpio_init_struct.Pin = GPIO_PIN_4;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;
    gpio_init_struct.Pull = GPIO_PULLUP;
    __HAL_RCC_SYSCFG_CLK_ENABLE(); 
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);
    
    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    
}
void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);
    
    if(GPIO_Pin == GPIO_PIN_4)
    {
        if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)==0)
        {
            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        }
    }
}