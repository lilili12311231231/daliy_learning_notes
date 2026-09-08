/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       template ʵ��
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� ̽���� F407������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
int main(void)
{
     HAL_Init(); /* ��ʼ�� HAL �� */
     sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
     delay_init(168); /* ��ʱ��ʼ�� */
     led_init(); /* ��ʼ�� LED */
     key_init();/* ��ʼ�� KEY */
    while(1)
    {
        if(key_scan())
        {
            
            HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
        }    
        else
        {
            delay_ms(10);
        }

    }
}