#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

/*������ʼ������*/
void key_init(void)
{

    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();


    gpio_init_struct.Pin = GPIO_PIN_0;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
}
/*����ɨ�躯��*/
uint8_t key_scan(uint8_t mode)
{
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
    {
        delay_ms(10); //��ʱ����
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
        {
            if (mode == 0) while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1);//等待按键松开
            return 1; //���·���1
        }
    }
    return 0; //û���·���0

}
