#include "./BSP/24CXX/24cxx.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IIC/myiic.h"
/**
* @brief 在 AT24CXX 指定地址写入一个数据
* @param addr: 写入数据的目的地址
* @param data: 要写入的数据
* @retval 无
*/
void at24cxx_init(void)
{
    iic_init();/* 初始化 IIC 总线 */
}
void at24cxx_write_one_byte(uint16_t addr, uint8_t data)
{
    iic_start();/* 发送起始信号 */
    if(EE_TYPE == AT24C16)/* 24C16 以上的型号, 分 2 个字节发送地址 */
    {
        iic_send_byte(0xA0);/* 发送器件地址+写信号 */
        iic_wait_ack();/* 等待应答信号 */
        iic_send_byte(addr >> 8);/* 发送高地址 */
    }
    else
    {
        /* 发送器件 0XA0 + 高位 a8/a9/a10 地址,写数据 */
        iic_send_byte(0XA0 + ((addr >> 8) << 1));/* 发送器件地址+写信号, 高地址在前 */
    }
    iic_wait_ack();/* 等待应答信号 */
    iic_send_byte(addr & 255);/* 发送低地址 */
    iic_wait_ack();/* 等待应答信号 */
    /* 因为写数据的时候,不需要进入接收模式了,所以这里不用重新发送起始信号了 */
    iic_send_byte(data);/* 发送 1 字节数据 */
    iic_wait_ack();/* 等待应答信号 */
    iic_stop();/* 发送停止信号 */
    delay_ms(10);/* 等待 10ms, 等待写入完成  EEPROM 写入比较慢,必须等到 10ms 后再写下一个字节*/
}
/**
* @brief 在 AT24CXX 指定地址读出一个数据
* @param readaddr: 开始读数的地址
* @retval 读到的数据
*/
uint8_t at24cxx_read_one_byte(uint16_t addr)
{
    uint8_t temp = 0;/* 临时变量 */
    iic_start();/* 发送起始信号 */
/* 根据不同的 24CXX 型号, 发送高位地址
* 1, 24C16 以上的型号, 分 2 个字节发送地址
 * 2, 24C16 及以下的型号, 分 1 个低字节地址 + 占用器件地址的 bit1~bit3 位 用于表示高位地
址, 最多 11 位地址
 * 对于 24C01/02, 其器件地址格式(8bit)为: 1 0 1 0 A2 A1 A0 R/W
 * 对于 24C04, 其器件地址格式(8bit)为: 1 0 1 0 A2 A1 a8 R/W
 * 对于 24C08, 其器件地址格式(8bit)为: 1 0 1 0 A2 a9 a8 R/W
 * 对于 24C16, 其器件地址格式(8bit)为: 1 0 1 0 a10 a9 a8 R/W
 * R/W : 读/写控制位 0,表示写; 1,表示读;
 * A0/A1/A2 : 对应器件的 1,2,3 引脚(只有 24C01/02/04/8 有这些脚)
 * a8/a9/a10: 对应存储整列的高位地址, 11bit 地址最多可以表示 2048 个位置,可以寻址
24C16 及以内的型号
 */
    if(EE_TYPE > AT24C16)/* 24C16 以上的型号, 分 2 个字节发送地址 */
    {
        iic_send_byte(0xA0);/* 发送器件地址+写信号 */
        iic_wait_ack();/* 等待应答信号 */
        iic_send_byte(addr >> 8);/* 发送高地址 */
    }
    else
    {
        /* 发送器件 0XA0 + 高位 a8/a9/a10 地址,写数据 */
        iic_send_byte(0XA0 + ((addr >> 8) << 1));/* 发送器件地址+写信号, 高地址在前 */
    }
    iic_wait_ack();/*  每次发送完一个字节,都要等待 ACK */
    iic_send_byte(addr & 255);/* 发送低地址 */
    iic_wait_ack();/*  等待 ACK, 此时地址发送完成了 */
    iic_start();/* 发送起始信号 */
    iic_send_byte(0XA1);/* 发送器件地址+读信号 */
    iic_wait_ack();/* 等待应答信号 */
    temp = iic_read_byte(0);/* 接收 1 字节数据 */
    iic_stop();/* 发送停止信号 */

    return temp;
}
/**
* @brief 检查 AT24CXX 是否正常
* @note 检测原理: 在器件的末地址写如 0X55, 然后再读取, 如果读取值为 0X55
* 则表示检测正常. 否则,则表示检测失败.
* @param 无
* @retval 检测结果
* 0: 检测成功
* 1: 检测失败
*/
uint8_t at24cxx_check(void)
{
    uint8_t temp; /* 临时变量 */
    uint16_t addr = EE_TYPE;/* 末地址 */
    temp = at24cxx_read_one_byte(addr); /* 读取末地址的数据 ,避免每次开机都写 AT24CXX*/
    if(temp == 0X55)/* 如果读取到的数据为 0X55,则表示检测正常 */
    {
        return 0;/* 返回检测成功 */
    }
    else /* 排除第一次初始化的情况 */
    {
        at24cxx_write_one_byte(addr, 0x55);/* 在末地址写入 0X55 */
        temp = at24cxx_read_one_byte(255); /* 读取末地址的数据 */
        if(temp == 0x55)return 0;/* 返回检测成功 */

    }
    return 1;/* 返回检测失败 */
}
/**
* @brief 在 AT24CXX 里面的指定地址开始读出指定个数的数据
* @param addr : 开始读出的地址 对 24c02 为 0~255
* @param pbuf : 数据数组首地址
560
STM32F407 开发指南
正点原子探索者 STM32F407 开发板教程
* @param datalen : 要读出数据的个数
* @retval 无
*/
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while(datalen--)
    {
        *pbuf++ = at24cxx_read_one_byte(addr++);/* 读取一个字节 */
    }
}
/**
* @brief 在 AT24CXX 里面的指定地址开始写入指定个数的数据
* @param addr : 开始写入的地址 对 24c02 为 0~255
* @param pbuf : 数据数组首地址
* @param datalen : 要写入数据的个数
* @retval 无
*/
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while(datalen--)
    {
        at24cxx_write_one_byte(addr++, *pbuf++);/* 写入一个字节 */
    }
}
