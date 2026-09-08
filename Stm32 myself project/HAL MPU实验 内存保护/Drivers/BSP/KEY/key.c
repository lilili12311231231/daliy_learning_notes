#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

#define KEY0   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)   /* KEY0 = PE4, active low */
#define KEY1   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)   /* KEY1 = PE3, active low */
#define KEY2   HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)   /* KEY2 = PE2, active low */
#define WK_UP  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)   /* WK_UP = PA0, active high */

void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* KEY0/KEY1/KEY2: input pull-up (pressed = low) */
    gpio_init_struct.Pin   = GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    /* WK_UP: input pull-down (pressed = high) */
    gpio_init_struct.Pin  = GPIO_PIN_0;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
}

uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;              /* key release flag */

    if (mode) key_up = 1;                   /* continuous mode */

    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1))
    {
        delay_ms(10);                       /* debounce */
        key_up = 0;

        if      (KEY0  == 0) return KEY0_PRES;
        else if (KEY1  == 0) return KEY1_PRES;
        else if (KEY2  == 0) return KEY2_PRES;
        else if (WK_UP == 1) return WKUP_PRES;
    }
    else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0)
    {
        key_up = 1;                         /* all released, allow next detect */
    }
    return 0;
}
