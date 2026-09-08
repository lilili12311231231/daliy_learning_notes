#include "./BSP/MPU/mpu.h"

/**
* @brief  �ϵ� MPU ���ã�Ĭ�Ͽ���������
*/
void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    /* 0. ��ʹ�� MemManage �쳣������Υ��������� HardFault */
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    /* 1. �ر� MPU */
    HAL_MPU_Disable();
    /* 2. Region 0��Flash 1MB���ɶ���ִ�� */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;/* Region 0 ʹ�� */
    MPU_InitStruct.Number           = MPU_REGION_NUMBER0;/* Region 0 */
    MPU_InitStruct.BaseAddress      = 0x08000000UL;/* Flash ��ʼ��ַ */
    MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;/* Flash ��С */
    MPU_InitStruct.SubRegionDisable = 0x00;/* �ر����������� */
    MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;/* 0 ����չ�ֶ� */
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;/* ȫȨ�� */
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;/* ִ��Ȩ�� */
    MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;/* ���ɹ��� */
    MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;/* ���ɻ��� */
    MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;/* ���ɻ���� */
    HAL_MPU_ConfigRegion(&MPU_InitStruct);/* ���� Region 0 */

    /* 3. Region 1��SRAM 128KB����ȨRW/����ȨR����ִ�� */
    MPU_InitStruct.Number           = MPU_REGION_NUMBER1;/* Region 1 */
    MPU_InitStruct.BaseAddress      = 0x20000000UL;/* SRAM1 ��ʼ��ַ */
    MPU_InitStruct.Size             = MPU_REGION_SIZE_128KB;/* SRAM1 ��С */
    MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW_URO;/* ��ȨRW/����ȨR */
    HAL_MPU_ConfigRegion(&MPU_InitStruct);/* ���� Region 1 */
    /* 4. Region 2�������� 512MB������ȨRW */
    MPU_InitStruct.Number           = MPU_REGION_NUMBER2;/* Region 2 */
    MPU_InitStruct.BaseAddress      = 0x40000000UL;/* ������ ��ʼ��ַ */
    MPU_InitStruct.Size             = MPU_REGION_SIZE_512MB;/* ������ ��С */
    MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RW;/* ��ȨRW */
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;/* ��ִ�� */
    HAL_MPU_ConfigRegion(&MPU_InitStruct);/* ���� Region 2 */
    /* 5. Region 3��16KB ��������Ĭ�Ͻ�ֹ���� */
    MPU_InitStruct.Number           = MPU_REGION_NUMBER3;/* Region 3 */
    MPU_InitStruct.BaseAddress      = PROTECTED_DATA_ADDR;/* ��������ʼ��ַ */
    MPU_InitStruct.Size             = MPU_REGION_SIZE_16KB;/* ��������С */
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;/* �޷���Ȩ�� */
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;/* ��ִ�� */
    HAL_MPU_ConfigRegion(&MPU_InitStruct);/* ���� Region 3 */
    /* 6. ʹ�� MPU + �������� */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}
/* ֻ���� Region 3 ��Ȩ�ޣ��������� Region��MPU ����ʹ��״̬�� */
static void MPU_SetRegion3(uint32_t access_permission)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;/* Region 3 ʹ�� */
    MPU_InitStruct.Number           = MPU_REGION_NUMBER3;/* Region 3 */
    MPU_InitStruct.BaseAddress      = PROTECTED_DATA_ADDR;/* ��������ʼ��ַ */
    MPU_InitStruct.Size             = MPU_REGION_SIZE_16KB;/* ��������С */
    MPU_InitStruct.AccessPermission = access_permission;/* ����Ȩ�� */
    MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_DISABLE;/* ��ִ�� */
    HAL_MPU_ConfigRegion(&MPU_InitStruct);/* ���� Region 3 */
}
/* ����������Region 3 ��Ϊ��ֹ���� */
void MPU_Protection_Enable(void)
{
    MPU_SetRegion3(MPU_REGION_NO_ACCESS);
}
/* �رձ�����Region 3 ��ΪȫȨ�� */
void MPU_Protection_Disable(void)
{
    MPU_SetRegion3(MPU_REGION_FULL_ACCESS);
}
/* д��������volatile ��ֹ���������Ż����� */
void MPU_Write(uint32_t data)
{
    volatile uint32_t *p = (volatile uint32_t *)PROTECTED_DATA_ADDR;
    *p = data;
}
/* �ӱ�������ȡ���� */
uint32_t MPU_Read(void)
{
    volatile uint32_t *p = (volatile uint32_t *)PROTECTED_DATA_ADDR;
    return *p;
}
/* fault ��ϣ���״̬�Ĵ�������ϵ������۲� */
void MPU_Fault_Diagnose(void)
{
    volatile uint32_t cfsr = SCB->CFSR;
    volatile uint32_t mmfar = SCB->MMFAR;
    volatile uint32_t bfar = SCB->BFAR;

    (void)cfsr;(void)mmfar;(void)bfar;/* ���Ż��������ϵ� */
    if((cfsr & SCB_CFSR_DACCVIOL_Msk) != 0)/* ���ݷ���Υ�� */
    {
        __NOP();/* �����ϵ㣬mmfar ����Υ���ַ */
    }
}
