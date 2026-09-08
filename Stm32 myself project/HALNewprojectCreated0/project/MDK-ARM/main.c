#include "./SYSTEM/sys/sys.h" 
#include "./SYSTEM/usart/usart.h" 
#include "./SYSTEM/delay/delay.h" 
 
void led_init(void); /* LED 初始化函数声明 */ 
 
int main(void) 
{ 
 HAL_Init(); /* 初始化 HAL 库 */ 
 sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */ 
 delay_init(168); /* 延时初始化 */ 
 led_init(); /* 初始化 LED */ 
 
 while(1) 
 { 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET); /* LED0 亮 */ 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET); /* LED1 灭 */ 
 delay_ms(500); 
 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET); /* LED0 灭 */ 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET); /* LED1 亮 */ 
 delay_ms(500); 
 } 
} 
 
/** 
 * @brief 初始化 LED 相关 IO 口, 并使能时钟 
 * @param 无 
 * @retval 无 
 */ 
void led_init(void) 
{ 
 GPIO_InitTypeDef gpio_init_struct; 
 
 __HAL_RCC_GPIOF_CLK_ENABLE(); /* LED0，LED1 时钟使能 */ 
 
 gpio_init_struct.Pin = GPIO_PIN_9; /* LED0 引脚 */ 
 gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */ 
 gpio_init_struct.Pull = GPIO_PULLUP; /* 上拉 */ 
 gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */ 
 HAL_GPIO_Init(GPIOF, &gpio_init_struct); /* 初始化 LED0 引脚 */ 
 
 gpio_init_struct.Pin = GPIO_PIN_1; /* LED1 引脚 */ 
 HAL_GPIO_Init(GPIOF, &gpio_init_struct); /* 初始化 LED1 引脚 */ 
 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 ,GPIO_PIN_SET); /* LED0 灭 */ 
 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET); /* LED1 灭 */ 

}
