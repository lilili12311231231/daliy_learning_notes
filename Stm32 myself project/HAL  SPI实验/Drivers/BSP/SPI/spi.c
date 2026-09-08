#include "./BSP/SPI/spi.h"

SPI_HandleTypeDef g_spi1_hander; /**< SPI1句柄 */
/**
 * @brief       SPI初始化代码
 *   @note      主机模式,8位数据,禁止硬件片选
 * @param       无
 * @retval      无
 */
 void spi1_init(void)
 {
    SPI1_SPI_CLK_ENABLE();/* SPI1时钟使能 */

    g_spi1_hander.Instance = SPI1_SPI; /* SPI1实例 */
    g_spi1_hander.Init.Mode = SPI_MODE_MASTER; /* 设置SPI工作模式，设置为主模式 */   
    g_spi1_hander.Init.Direction = SPI_DIRECTION_2LINES; /* 设置SPI单向或双向的数据模式:SPI设置为双线双向全双工 */
    g_spi1_hander.Init.DataSize = SPI_DATASIZE_8BIT; /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
    g_spi1_hander.Init.CLKPolarity = SPI_POLARITY_HIGH; /* 设置SPI时钟极性:SPI时钟高电平有效 */
    g_spi1_hander.Init.CLKPhase = SPI_PHASE_2EDGE; /* 设置SPI时钟相位:数据捕获于第二个时钟沿 */
    g_spi1_hander.Init.NSS = SPI_NSS_SOFT; /* 设置NSS信号模式:SPI软件管理NSS信号 */
    g_spi1_hander.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* 设置波特率预分频的值:波特率预分频值为256 */
    g_spi1_hander.Init.FirstBit = SPI_FIRSTBIT_MSB; /* 设置数据传输从MSB位开始:数据传输从MSB位开始 */
    g_spi1_hander.Init.TIMode = SPI_TIMODE_DISABLE; /* 禁用TI模式:不使用TI模式 */
    g_spi1_hander.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; /* 禁用硬件CRC计算:不使用CRC功能 */
    g_spi1_hander.Init.CRCPolynomial = 7; /* 设置CRC多项式:默认值为7 */
    HAL_SPI_Init(&g_spi1_hander);

    __HAL_SPI_ENABLE(&g_spi1_hander);/* 使能SPI外设 */
    spi1_read_write_byte(0xFF);/* 发送一个空字节,清空SPI发送缓冲区 */

}
/**
 * @brief       SPI1底层驱动，时钟使能，引脚配置
 *   @note      此函数会被HAL_SPI_Init()调用
 * @param       hspi:SPI句柄
 * @retval      无
 */
 void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
 {
    GPIO_InitTypeDef gpio_init_struct; /**< GPIO初始化结构体 */
    if(hspi->Instance == SPI1_SPI)
    {
        SPI1_SCK_GPIO_CLK_ENABLE();/* 使能SCK引脚时钟 */
        SPI1_MISO_GPIO_CLK_ENABLE();/* 使能MISO引脚时钟 */
        SPI1_MOSI_GPIO_CLK_ENABLE();/* 使能MOSI引脚时钟 */
        /* SCK引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI1_SCK_GPIO_PIN; /* 配置SCK引脚 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP; /* 设置为复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP; /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; /* 设置GPIO速度为高速 */
        gpio_init_struct.Alternate = GPIO_AF5_SPI1; /* 设置复用为SPI1 */
        HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &gpio_init_struct); /* 初始化SCK引脚 */
        /* MISO引脚模式设置(复用输入) */
        gpio_init_struct.Pin = SPI1_MISO_GPIO_PIN; /* 配置MISO引脚 */
        HAL_GPIO_Init(SPI1_MISO_GPIO_PORT, &gpio_init_struct); /* 初始化MISO引脚 */
        /* MOSI引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI1_MOSI_GPIO_PIN; /* 配置MOSI引脚 */
        HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &gpio_init_struct); /* 初始化MOSI引脚 */
    }

}
/**
 * @brief       SPI1速度设置函数
 *   @note      SPI1时钟选择来自APB1, 即PCLK1, 为 42MHz
 *              SPI速度 = PCLK1 / 2^(speed + 1)
 * @param       speed   : SPI1时钟分频系数
                        取值为SPI_BAUDRATEPRESCALER_2~SPI_BAUDRATEPRESCALER_2 256
 * @retval      无
 */
 void spi1_set_speed(uint8_t speed)
 {
    assert_param(IS_SPI_BAUDRATE_PRESCALER(speed));/* 检查参数有效性 */
    __HAL_SPI_DISABLE(&g_spi1_hander);/* 先关闭SPI1 */
    g_spi1_hander.Instance->CR1 &= 0XFFC7;/* 位3-5清零清除原来的分频系数 */
    g_spi1_hander.Instance->CR1 |= (uint32_t)(speed << 3);/* 设置新的分频系数设置SPI速度 */
    __HAL_SPI_ENABLE(&g_spi1_hander);/* 使能SPI1 */
 }
/**
 * @brief       SPI1读写一个字节数据
 * @param       txdata  : 要发送的数据(1字节)
 * @retval      接收到的数据(1字节)
 */
 uint8_t spi1_read_write_byte(uint8_t txdata)
 {
    uint8_t rxdata = 0;/* 接收数据 */
    HAL_SPI_TransmitReceive(&g_spi1_hander, &txdata, &rxdata, 1, 1000);/* 发送并接收数据 */
    return rxdata;/* 返回接收到的数据 */
 }
 