#ifndef __SPI_H__
#define __SPI_H__

#include "./SYSTEM/sys/sys.h"
/**
 * @brief SPI1引脚定义
 * 
 */

 #define SPI1_SCK_GPIO_PORT GPIOB
 #define SPI1_SCK_GPIO_PIN GPIO_PIN_3
 #define SPI1_SCK_GPIO_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)/*< GPIOB时钟使能 */
 
#define SPI1_MISO_GPIO_PORT GPIOB
#define SPI1_MISO_GPIO_PIN GPIO_PIN_4
#define SPI1_MISO_GPIO_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)/*< GPIOB时钟使能 */

 #define SPI1_MOSI_GPIO_PORT GPIOB
 #define SPI1_MOSI_GPIO_PIN GPIO_PIN_5
 #define SPI1_MOSI_GPIO_CLK_ENABLE()   do{__HAL_RCC_GPIOB_CLK_ENABLE();}while(0)/*< GPIOB时钟使能 */
 /**
 * @brief SPI1相关定义
 * 
 */
 #define SPI1_SPI SPI1 /**< SPI1实例 */
 #define SPI1_SPI_CLK_ENABLE()   do{__HAL_RCC_SPI1_CLK_ENABLE();}while(0)/*< SPI1时钟使能 */

 /* SPI1 通信速度设置，fPCLK = 84MHz */
#define SPI_SPEED_2         0   /* 84/2   = 42Mhz    */
#define SPI_SPEED_4         1   /* 84/4   = 21Mhz    */
#define SPI_SPEED_8         2   /* 84/8   = 10.5Mhz  */
#define SPI_SPEED_16        3   /* 84/16  = 5.25Mhz  */
#define SPI_SPEED_256       4   /* 84/256 = 328.125Khz */
#define SPI_SPEED_128       5   /* 84/128 = 656.25Khz  */
#define SPI_SPEED_64        6   /* 84/64  = 1.3125Mhz  */
#define SPI_SPEED_32        7   /* 84/32  = 2.625Mhz   */

 void spi1_init(void);
 void spi1_set_speed(uint8_t speed);
 uint8_t spi1_read_write_byte(uint8_t txdata);

#endif
