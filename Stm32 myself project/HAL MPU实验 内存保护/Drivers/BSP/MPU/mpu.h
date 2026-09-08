#ifndef __MPU_H
#define __MPU_H
#include "stm32f4xx_hal.h"
#include "./SYSTEM/sys/sys.h"

#define PROTECTED_DATA_ADDR   0x20010000UL   /* 保护区起始（SRAM1 中部 16KB） */

void MPU_Config(void);/* 上电配置，默认保护开启 */
void MPU_Protection_Enable(void);/* 保护开启 */
void MPU_Protection_Disable(void);/* 保护关闭 */
void MPU_Write(uint32_t data);/* 写入数据到指定地址 */
uint32_t MPU_Read(void);/* 从指定地址读取数据 */
void MPU_Fault_Diagnose(void);/* 故障诊断 */

#endif
