#include "stm32f4xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/MPU/mpu.h"

void SystemClock_Config(void);
int main(void)
{
    uint8_t key;
    uint8_t mpu_protected = 1;/* �ϵ�Ĭ�ϱ������� */
    uint32_t tick = 0;

    HAL_Init();
    SystemClock_Config();
    delay_init(168);
    led_init();
    key_init();
    usart_init(115200);
    MPU_Config(); /* ���� MPU��Ĭ�ϱ������� */
    printf("MPU Configured\r\n");
    while(1)
    {
        key  = key_scan(0);/* ����ģʽɨ�� */
        /* ----- WK_UP���л��������� ----- */
        if(key == WKUP_PRES)
        {
            if(mpu_protected)
            {
                MPU_Protection_Disable();
                mpu_protected = 0;
                printf("MPU unprotected\r\n");/* ��ӡ����״̬ */
            }
            else
            {
                MPU_Protection_Enable();
                mpu_protected = 1;
                printf("MPU protected\r\n");/* ��ӡ����״̬ */
            }
        }
        /* ----- KEY0��д������ ----- */
        else if(key == KEY0_PRES)
        {
            printf("Write 0x55 to protected area\r\n");
            MPU_Write(0x55);/* ��������ʱ���ﴥ�� fault */
            printf("Write success\r\n");/* �����رղ����ߵ��� */
        }
        /* ----- KEY1���������� ----- */
        else if(key == KEY1_PRES)
        {
            uint32_t val;
            printf("Read protected area\r\n");
            val = MPU_Read();/* ��������ʱ����Ҳ fault */
            printf("Read value = 0x%08X\r\n", (unsigned int)val);
        }
       /* ----- LED0 ÿ 500ms ��תһ�Σ���ʾ�������� ----- */
       if(++tick >= 50)
       {
           tick = 0;
           LED0_TOGGLE();
       }
       HAL_Delay(10); /* 10ms ѭ������ */

    }
}
/* ϵͳʱ�ӣ�8MHz HSE �� PLL �� 168MHz */
void SystemClock_Config(void) /* ϵͳʱ������ */
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    __HAL_RCC_PWR_CLK_ENABLE();/* ʹ�� PWR ʱ�� */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);/* ���� PWR ��ѹ�ȼ�Ϊ 1 */

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSE;/* ʹ�� HSE */
    RCC_OscInitStruct.HSEState            = RCC_HSE_ON;/* HSE ʹ�� */
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;/* PLL ʹ�� */
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSE;/* PLL ʱ����Դ HSE */
    RCC_OscInitStruct.PLL.PLLM            = 8;/* PLLM = 8 */
    RCC_OscInitStruct.PLL.PLLN            = 336;/* PLLN = 336 */
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;/* PLLP = 2 */
    RCC_OscInitStruct.PLL.PLLQ            = 7;/* PLLQ = 7 */
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;/* ʹ�� HCLK��SYSCLK��PCLK1��PCLK2 */
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;/* SYSCLK ʱ����Դ PLL */
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;/* AHB ʱ�ӷ�ƵΪ 1 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;/* APB1 ʱ�ӷ�ƵΪ 4 */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;/* APB2 ʱ�ӷ�ƵΪ 2 */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);/* ����ʱʱ�� */
}




