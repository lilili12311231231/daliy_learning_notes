#include "./BSP/LED/led.h"

/* LED0 引脚定义 */
#define LED0_GPIO_PORT GPIOF
#define LED0_GPIO_PIN GPIO_PIN_9
#define LED0_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)
/* LED1 引脚定义 */
#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_PIN GPIO_PIN_10
#define LED1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)

void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    LED0_GPIO_CLK_ENABLE(); /* LED0 时钟使能 */
    LED1_GPIO_CLK_ENABLE(); /* LED1 时钟使能 */
    
    gpio_init_struct.Pin = LED0_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.Pin   = LED1_GPIO_PIN;
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);
    
    LED0(1);
    LED1(1);
 }
  
    
    