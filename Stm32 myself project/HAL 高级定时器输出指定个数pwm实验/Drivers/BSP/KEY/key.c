#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

/*按键初始化函数*/
void key_init(void)
{
    
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    
    gpio_init_struct.Pin = GPIO_PIN_2;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);  
}
/*按键扫描函数*/
uint8_t key_scan(void)
{
    if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0)
    {
        delay_ms(10); //延时消抖//消抖
            if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0)
            {
                while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) == 0);//等待按键松开
                return 1; //按下返回1
            }
    }
    return 0; //没按下返回0
    
}
