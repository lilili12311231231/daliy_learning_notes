#ifndef __WDG_H
#define __WDG_H

#include "./SYSTEM/sys/sys.h"

void iwdg_init(uint8_t Prer, uint16_t rlr);                                                                          /* 初始化 */
void iwdg_feed(void);
#endif
