#include "./BSP/RS485/rs485.h"
#include "./SYSTEM/delay/delay.h"

UART_HandleTypeDef g_rs485_handler; /* RS485 RS485处理句柄（串口） */

#ifdef RS485_EN_RX /* 使能RS485接收 */

uint8_t g_RS485_rx_buf[RS485_REC_LEN];   /* 接收缓冲,最大RS485_REC_LEN个字节 */
uint8_t g_RS485_rx_cnt = 0;                  /* 接收数据长度 */

void RS485_UX_IRQHandler(void)
{
    uint8_t res;

    if((__HAL_UART_GET_FLAG(&g_rs485_handler, UART_FLAG_RXNE) != RESET)) /* 接收中断标志位 */
    {
        HAL_UART_Receive(&g_rs485_handler, &res, 1, 1000);
        if(g_RS485_rx_cnt < RS485_REC_LEN ) /* 接收缓冲区未满 */
        {
            g_RS485_rx_buf[g_RS485_rx_cnt++] = res; /* 记录接收数据 */

        }
    }
}
#endif

/**
* @brief       RS485初始化函数
*   @note      该函数主要是初始化串口
* @param       baudrate: 波特率, 根据自己需要设置波特率值
* @retval      无
*/
void rs485_init(uint32_t baudrate)
{
    /* 初始化串口 */
    RS485_RE_GPIO_CLK_ENABLE(); /* 使能RS485_RE_GPIO时钟 */
    RS485_TX_GPIO_CLK_ENABLE(); /* 使能RS485_TX_GPIO时钟 */
    RS485_RX_GPIO_CLK_ENABLE(); /* 使能RS485_RX_GPIO时钟 */
    RS485_UX_CLK_ENABLE(); /* 使能RS485_UX时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = RS485_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF7_USART2; /* 选择USART2 */
    HAL_GPIO_Init(RS485_TX_GPIO_PORT, &gpio_initure); /* 串口TX脚，模式设置 */

    gpio_initure.Pin = RS485_RX_GPIO_PIN; /* 串口RX脚 */
    HAL_GPIO_Init(RS485_RX_GPIO_PORT, &gpio_initure); /* 串口RX脚，模式设置 */

    gpio_initure.Pin = RS485_RE_GPIO_PIN; /* 串口RE脚 */
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP; /* 串口RE脚，模式设置 */
    gpio_initure.Pull = GPIO_PULLUP; /* 串口RE脚，上拉设置 */
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH; /* 串口RE脚，速度设置 */
    HAL_GPIO_Init(RS485_RE_GPIO_PORT, &gpio_initure); /* 串口RE脚，模式设置 */

    /* USART 初始化设置 */
    g_rs485_handler.Instance = RS485_UX; /*选择485对应串口*/
    g_rs485_handler.Init.BaudRate = baudrate; /* 波特率设置 */
    g_rs485_handler.Init.WordLength = UART_WORDLENGTH_8B; /* 字长设置8位数据帧 */
    g_rs485_handler.Init.StopBits = UART_STOPBITS_1; /* 停止位设置1位 */
    g_rs485_handler.Init.Parity = UART_PARITY_NONE; /* 校验位设置无 */
    g_rs485_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* 硬件流控制设置无 */
    g_rs485_handler.Init.Mode = UART_MODE_TX_RX; /* 串口模式设置为发送和接收 */
    HAL_UART_Init(&g_rs485_handler); /* 收发模式 */
    __HAL_UART_DISABLE_IT(&g_rs485_handler, UART_IT_TC); /* 使能串口 */

#if RS485_EN_RX/* 如果使能了接收 */
                /* 使能接收中断 */
    __HAL_UART_ENABLE_IT(&g_rs485_handler, UART_IT_RXNE); /* 使能接收中断 */
    HAL_NVIC_EnableIRQ(RS485_UX_IRQn); /* 使能RS485_UX中断 */
    HAL_NVIC_SetPriority(RS485_UX_IRQn, 1, 3); /* 设置RS485_UX中断优先级 */
#endif

    RS485_RE(0); /*默认接收模式*/

}
/**
 * @brief       RS485发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 RS485_REC_LEN 个字节)
 * @retval      无
 */
void rs485_send_data(uint8_t *buf, uint8_t len)
{
    RS485_RE(1); /* 发送模式 */
    HAL_UART_Transmit(&g_rs485_handler, buf, len, 1000);/* 发送数据 */
    RS485_RE(0); /* 接收模式 */
}
/**
 * @brief       RS485查询接收到的数据
 * @param       buf     : 接收缓冲区首地址
 * @param       len     : 接收到的数据长度
 *   @arg               0   , 表示没有接收到任何数据
 *   @arg               其他, 表示接收到的数据长度
 * @retval      无
 */
void rs485_receive_data(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen = g_RS485_rx_cnt;
    uint8_t i = 0;
    *len = 0; /*默认为0 */
    delay_ms(10); /* 延时10ms, 连续超过10ms没有接收到一个数据,等待接收完成 */

    if(rxlen == g_RS485_rx_cnt && rxlen) /* 接收到了数据,且接收完成了 */
    {
        for(i = 0; i < rxlen; i++)
        {
            buf[i] = g_RS485_rx_buf[i]; /* 复制接收数据到接收缓冲区 */
        }
        *len = g_RS485_rx_cnt; /* 记录本次数据长度 */
        g_RS485_rx_cnt = 0; /* 接收数据长度清0 */
    }
}
