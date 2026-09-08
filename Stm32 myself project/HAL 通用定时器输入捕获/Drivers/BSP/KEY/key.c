#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    WK_UP_CLK_ENABLE();
    KEY0_CLK_ENABLE();
    KEY1_CLK_ENABLE();
    KEY2_CLK_ENABLE();

    /* WK_UP (PA0) : active high, pull-down */
    gpio_init_struct.Pin = WK_UP_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(WK_UP_GPIO_PORT, &gpio_init_struct);

    /* KEY0 (PE4) : active low, pull-up */
    gpio_init_struct.Pin = KEY0_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);

    /* KEY1 (PE3) : active low, pull-up */
    gpio_init_struct.Pin = KEY1_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);

    /* KEY2 (PE2) : active low, pull-up */
    gpio_init_struct.Pin = KEY2_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);
}

uint8_t key_scan(void)
{
    /* WK_UP: active high (returns 1 when pressed) */
    if (HAL_GPIO_ReadPin(WK_UP_GPIO_PORT, WK_UP_GPIO_PIN) == 1)
    {
        delay_ms(10);
        if (HAL_GPIO_ReadPin(WK_UP_GPIO_PORT, WK_UP_GPIO_PIN) == 1)
        {
            while (HAL_GPIO_ReadPin(WK_UP_GPIO_PORT, WK_UP_GPIO_PIN) == 1);
            return WK_UP_PRESS;
        }
    }

    /* KEY0: active low (returns 1 when pressed) */
    if (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == 0)
    {
        delay_ms(10);
        if (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == 0)
        {
            while (HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN) == 0);
            return KEY0_PRESS;
        }
    }

    /* KEY1: active low */
    if (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
    {
        delay_ms(10);
        if (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
        {
            while (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0);
            return KEY1_PRESS;
        }
    }

    /* KEY2: active low */
    if (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0)
    {
        delay_ms(10);
        if (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0)
        {
            while (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == 0);
            return KEY2_PRESS;
        }
    }

    return 0;
}
