#ifndef __OLED_H
#define __OLED_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/OLED/oledfont.h"

/******************************************************************************************/
/* OLED 8080 并口引脚定义 */

#define OLED_RS_GPIO_PORT               GPIOA
#define OLED_RS_GPIO_PIN                GPIO_PIN_4
#define OLED_RS(x)      do{ x ? \
                            HAL_GPIO_WritePin(OLED_RS_GPIO_PORT, OLED_RS_GPIO_PIN, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(OLED_RS_GPIO_PORT, OLED_RS_GPIO_PIN, GPIO_PIN_RESET); \
                        }while(0)

 #define OLED_WR_GPIO_PORT               GPIOB
#define OLED_WR_GPIO_PIN                GPIO_PIN_7
#define OLED_WR(x)      do{ x ? \
                            HAL_GPIO_WritePin(OLED_WR_GPIO_PORT, OLED_WR_GPIO_PIN, GPIO_PIN_SET) : \
                                 HAL_GPIO_WritePin(OLED_WR_GPIO_PORT, OLED_WR_GPIO_PIN, GPIO_PIN_RESET); \
                             }while(0)

#define OLED_RD_GPIO_PORT               GPIOD
#define OLED_RD_GPIO_PIN                GPIO_PIN_6
#define OLED_RD(x)      do{ x ? \
                            HAL_GPIO_WritePin(OLED_RD_GPIO_PORT, OLED_RD_GPIO_PIN, GPIO_PIN_SET) : \
                                 HAL_GPIO_WritePin(OLED_RD_GPIO_PORT, OLED_RD_GPIO_PIN, GPIO_PIN_RESET); \
                             }while(0)

#define OLED_CS_GPIO_PORT               GPIOD
#define OLED_CS_GPIO_PIN                GPIO_PIN_7
#define OLED_CS(x)      do{ x ? \
                            HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_GPIO_PIN, GPIO_PIN_SET) : \
                                 HAL_GPIO_WritePin(OLED_CS_GPIO_PORT, OLED_CS_GPIO_PIN, GPIO_PIN_RESET); \
                             }while(0)

#define OLED_RST_GPIO_PORT              GPIOG
#define OLED_RST_GPIO_PIN               GPIO_PIN_15
#define OLED_RST(x)     do{ x ? \
                            HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_GPIO_PIN, GPIO_PIN_SET) : \
                                 HAL_GPIO_WritePin(OLED_RST_GPIO_PORT, OLED_RST_GPIO_PIN, GPIO_PIN_RESET); \
                             }while(0)

/******************************************************************************************/

#define OLED_CMD        0       /* 命令 */
#define OLED_DATA       1       /* 数据 */

     /* 函数声明 */
void oled_init(void);
void oled_refresh_gram(void);
void oled_clear(void);
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
void oled_show_string(uint8_t x, uint8_t y, uint8_t *str, uint8_t size);
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
#endif
