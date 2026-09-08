#include "./BSP/IIC/myiic.h"


void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;/* GPIO初始化结构体 */

    IIC_SCL_GPIO_CLK_ENABLE();/* 使能IIC SCL对应的GPIO时钟 */
    IIC_SDA_GPIO_CLK_ENABLE();/* 使能IIC SDA对应的GPIO时钟 */

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;/* 选择IIC SCL对应的GPIO引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;/* 选择IIC SCL对应的GPIO引脚为开漏输出模式 */
    gpio_init_struct.Pull = GPIO_PULLUP;/* 选择IIC SCL对应的GPIO引脚为上拉模式 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;/* 选择IIC SCL对应的GPIO引脚为高速模式 */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct);/* 初始化IIC SCL对应的GPIO引脚 */
/* SDA 引脚模式设置,开漏输出,上拉, 这样就不用再设置 IO 方向了, 开漏输出的时候(=1),
也可以读取外部信号的高低电平 */
    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;/* 选择IIC SDA对应的GPIO引脚 */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);/* 初始化IIC SDA对应的GPIO引脚 */
    iic_stop();/* 发送IIC停止信号, 复位IIC总线上的所有设备 */
}
/* IIC延时函数 */
static void iic_delay(void)
{
    delay_us(2);/* 2us 的延时, 读写速度在 250Khz 以内 */
}
/*产生 IIC 起始信号*/
void iic_start(void)
{
    IIC_SDA(1);/* 发送起始信号前, 保证SDA和SCL都为高电平 */
    IIC_SCL(1);/* 保证SDA和SCL都为高电平 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SDA(0);/* START: 当SCL高电平时, SDA由高变低 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(0);/* 钳住I2C总线, 准备发送或接收数据 */
    iic_delay();/* 发送完起始信号后, 延时一段时间, 等待总线稳定 */
}
/*产生 IIC 停止信号*/
void iic_stop(void)
{
    IIC_SDA(0);/* 发送停止信号前, 保证SDA和SCL都为低电平 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(1);/* 当SCL高电平时, SDA由低变高, 产生停止信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SDA(1); /* 发送停止信号后, 延时一段时间, 等待总线稳定 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
}
/* IIC 发送一个字节 data: 要发送的数据 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;/* 临时变量 */
    for(t = 0; t < 8; t++) /* 发送8位数据 */
    {
        IIC_SDA((data & 0x80) >> 7);/* 先发送数据的高位 */
        iic_delay();/* 延时一段时间, 等待总线稳定 */
        IIC_SCL(1);/* 拉高时钟线, 让接收方读取数据 */
        iic_delay();/* 延时一段时间, 等待总线稳定 */
        IIC_SCL(0);/* 拉低时钟线, 让接收方准备接收数据 */
        data <<= 1;/* 左移一位, 准备发送下一位数据 */
    }
    IIC_SDA(1);/* 发送完成, 主机释放 SDA 线 */
}
/**
* @brief IIC 读取一个字节
* @param ack: ack=1 时，发送 ack; ack=0 时，发送 nack
* @retval 接收到的数据
*/
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;/* 临时变量 */
    for(i = 0; i < 8; i++) /* 读取8位数据 */
    {
        receive <<= 1;/* 高位先输出, 所以先收到的数据位要左移 */
        IIC_SCL(1);/* 拉高时钟线, 让接收方读取数据 */
        iic_delay();/* 延时一段时间, 等待总线稳定 */
        if(IIC_READ_SDA)/* 读取 SDA 线的电平 */
        {
            receive++;/* 如果 SDA 线为高电平, 则接收到的数据为1 */
        }
        IIC_SCL(0);/* 拉低时钟线, 让接收方准备接收数据 */
        iic_delay();/* 延时一段时间, 等待总线稳定 */
    }
    if(!ack)
    {
        iic_nack();/* 发送 nack 信号, 表示接收完成 */
    }
    else
    {
        iic_ack();/* 发送 ack 信号, 表示继续接收数据 */
    }
    return receive;/* 返回接收到的数据 */
}
/**
* @brief 等待应答信号到来
* @param 无
* @retval 1，接收应答失败
* 0，接收应答成功
*/
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;/* 等待时间计数器 */
    uint8_t rack = 0;/* 接收的应答信号 */

    IIC_SDA(1);/* 释放 SDA 线, 准备接收应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(1);/* 拉高时钟线, 让接收方发送应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */

    while(IIC_READ_SDA) /* 等待应答信号到来 */
    {
        waittime++;/* 等待时间计数器加1 */
        if(waittime > 250)/* 如果等待时间超过250次, 则认为接收应答失败 */
        {
            iic_stop();/* 发送IIC停止信号, 复位IIC总线上的所有设备 */
            rack = 1;/* 接收应答失败 */
            break;/* 跳出循环 */
        }
    }
    IIC_SCL(0);/* 拉低时钟线, 让接收方准备接收数据 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    return rack;/* 返回接收应答信号的状态 */
}
/**
* @brief 产生 ACK 应答
* @param 无
* @retval 无
*/
void iic_ack(void)
{
    IIC_SDA(0); /* 拉低 SDA 线, 产生 ACK 应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(1);/* 拉高时钟线, 让接收方读取 ACK 应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(0);/* 拉低时钟线, 让接收方准备接收数据 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SDA(1);/* 主机释放 SDA 线 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
}
/**
* @brief 不产生 ACK 应答
* @param 无
* @retval 无
*/
void iic_nack(void)
{
    IIC_SDA(1);/* 拉高 SDA 线, 不产生 ACK 应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(1);/* 拉高时钟线, 让接收方读取 NACK 应答信号 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
    IIC_SCL(0);/* 拉低时钟线, 让接收方准备接收数据 */
    iic_delay();/* 延时一段时间, 等待总线稳定 */
}