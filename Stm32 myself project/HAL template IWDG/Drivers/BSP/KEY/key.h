#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

#define WKUP_PRES  1    /* KEY_UP 按下标志 */

void key_init(void);
uint8_t key_scan(uint8_t mode);

#endif
