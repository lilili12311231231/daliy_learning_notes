#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

#define KEY0_PRES  1                       /* KEY0 pressed */
#define KEY1_PRES  2                       /* KEY1 pressed */
#define KEY2_PRES  3                       /* KEY2 pressed */
#define WKUP_PRES  4                       /* WK_UP pressed */

void key_init(void);
uint8_t key_scan(uint8_t mode);             /* mode=0: single press, mode=1: continuous */

#endif
