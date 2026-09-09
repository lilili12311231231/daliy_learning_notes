/*  CAN 驱动代码 */
#include "./BSP/CAN/can.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

CAN_HandleTypeDef g_canx_handler; /* CAN 处理柄 */
CAN_TxHeaderTypeDef g_canx_txheader; /* CAN 发送头参数句柄 */
CAN_RxHeaderTypeDef g_canx_rxheader; /* CAN 接收头参数句柄 */

/*CAN 初始化函数*/

/**
 * @brief       CAN初始化
 * @param       tsjw    : 重新同步跳跃时间单元.范围: 1~3;
 * @param       tbs2    : 时间段2的时间单元.范围: 1~8;
 * @param       tbs1    : 时间段1的时间单元.范围: 1~16;
 * @param       brp     : 波特率分频器.范围: 1~1024;
 *   @note      以上4个参数, 在函数内部会减1, 所以, 任何一个参数都不能等于0
 *              CAN挂在APB1上面, 其输入时钟频率为 Fpclk1 = PCLK1 = 42Mhz
 *              tq     = brp * tpclk1;
 *              波特率 = Fpclk1 / ((tbs1 + tbs2 + 1) * brp);
 *              我们设置 can_init(1, 6, 7, 6, 1), 则CAN波特率为:
 *              42M / ((6 + 7 + 1) * 6) = 500Kbps
 *
 * @param       mode    : CAN_MODE_NORMAL,  普通模式;
                          CAN_MODE_LOOPBACK,回环模式;
 * @retval      0,  初始化成功; 其他, 初始化失败;
 */

 uint8_t can_init(uint32_t tsjw, uint32_t tbs2, uint32_t tbs1, uint16_t brp, uint32_t mode) 
{
    g_canx_handler.Instance = CAN1; /* 选择CAN1 */
    g_canx_handler.Init.Prescaler = brp ; /* 波特率分频器 分频系数(Fdiv)为brp+1 */
    g_canx_handler.Init.Mode = mode; /* CAN模式 */
    g_canx_handler.Init.SyncJumpWidth = tsjw; /* 重新同步跳跃时间单元 */
    g_canx_handler.Init.TimeSeg1 = tbs1; /* 时间段1的时间单元 */
    g_canx_handler.Init.TimeSeg2 = tbs2; /* 时间段2的时间单元 */    
    g_canx_handler.Init.TimeTriggeredMode = DISABLE; /* 非时间触发模式 */
    g_canx_handler.Init.AutoBusOff = DISABLE; /* 软件自动离线管理 */
    g_canx_handler.Init.AutoWakeUp = DISABLE;  /* 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位) */
    g_canx_handler.Init.AutoRetransmission = ENABLE; /* 自动重传 */
    g_canx_handler.Init.ReceiveFifoLocked = DISABLE; /* 报文不锁定,新的覆盖旧的接收FIFO锁定模式 */
    g_canx_handler.Init.TransmitFifoPriority = DISABLE; /* 优先级由报文标识符决定发送FIFO优先级 */
    if(HAL_CAN_Init(&g_canx_handler) != HAL_OK)
    {
        return 1;
    }

    #if CAN_RX0_INT_ENABLE

    /* 使用中断接收 */ 
    __HAL_CAN_ENABLE_IT(&g_canx_handler, CAN_IT_RX_FIFO0_PENDING); /* FIFO0消息挂号中断允许 */
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn); /* 使能CAN1_RX0_IRQn中断 */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);/* 抢占优先级1，子优先级0 */
#endif

    CAN_FilterTypeDef sFilterConfig; /* CAN 过滤器配置 */
    /* 配置CAN过滤器 */
    sFilterConfig.FilterBank = 0; /* 过滤器0 */
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK; /* ID掩码模式 */
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; /* 32位掩码 */
    sFilterConfig.FilterIdHigh = 0x0000; /*32位高16位掩码, 0x0000 表示所有ID都通过*/
    sFilterConfig.FilterIdLow = 0x0000; /*32位低16位掩码, 0x0000 表示所有ID都通过*/
    sFilterConfig.FilterMaskIdHigh = 0x0000;             /* 32位掩码高位 */
    sFilterConfig.FilterMaskIdLow = 0x0000;              /* 32位掩码低位 */
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; /* 过滤器0 分配给FIFO0 */
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; /* 使能过滤器0 */
    sFilterConfig.SlaveStartFilterBank = 14; /* 从过滤器14 */

    /* CAN过滤器配置 */
    if(HAL_CAN_ConfigFilter(&g_canx_handler, &sFilterConfig) != HAL_OK)
    {
        return 2;
    }

    /* 启动CAN外围设备 */
    if(HAL_CAN_Start(&g_canx_handler) != HAL_OK)
    {
        return 3;
    }

    return 0;
}
/**
 * @brief       CAN底层驱动，引脚配置，时钟配置，中断配置
                此函数会被HAL_CAN_Init()调用
 * @param       hcan:CAN句柄
 * @retval      无
 */
 void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
 {
    if(CAN1 == hcan->Instance)
    {
        CAN_RX_GPIO_CLK_ENABLE();/* CAN_RX脚时钟使能 */
        CAN_TX_GPIO_CLK_ENABLE();/* CAN_TX脚时钟使能 */
        __HAL_RCC_CAN1_CLK_ENABLE();/* CAN1时钟使能 */

        GPIO_InitTypeDef gpio_init_struct;

        gpio_init_struct.Pin = CAN_TX_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(CAN_TX_GPIO_PORT, &gpio_init_struct); /* CAN_TX脚 模式设置 */

        gpio_init_struct.Pin = CAN_RX_GPIO_PIN;
        HAL_GPIO_Init(CAN_RX_GPIO_PORT, &gpio_init_struct); /* CAN_RX脚 模式设置 必须设置成输入模式*/

    }
 }
#if CAN_RX0_INT_ENABLE /* 使能RX0中断 */

/**
 * @brief       CAN RX0 中断服务函数
 *   @note      处理CAN FIFO0的接收中断
 * @param       无
 * @retval      无
 */
 void CAN1_RX0_IRQHandler(void)
 {
    uint8_t rxbuf[8];
    uint32_t id;
    can_receive_msg(id, rxbuf); /* 接收CAN消息 */
    printf("id:%d\r\n",g_canx_rxheader.StdId);
    printf("ide:%d\r\n",g_canx_rxheader.IDE);
    printf("rtr:%d\r\n", g_canx_rxheader.RTR);
    printf("len:%d\r\n", g_canx_rxheader.DLC);

    printf("rxbuf[0]:%d\r\n", rxbuf[0]);
    printf("rxbuf[1]:%d\r\n", rxbuf[1]);
    printf("rxbuf[2]:%d\r\n", rxbuf[2]);
    printf("rxbuf[3]:%d\r\n", rxbuf[3]);
    printf("rxbuf[4]:%d\r\n", rxbuf[4]);
    printf("rxbuf[5]:%d\r\n", rxbuf[5]);
    printf("rxbuf[6]:%d\r\n", rxbuf[6]);
    printf("rxbuf[7]:%d\r\n", rxbuf[7]);
 }
 #endif


/**
 * @brief       CAN 发送一组数据
 *   @note      发送格式固定为: 标准ID, 数据帧
 * @param       id      : 标准ID(11位)
 * @param       msg     : 数据指针
 * @param       len     : 数据长度
 * @retval      发送状态 0, 成功; 1, 失败;
 */
 uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len)
 {
    uint16_t t = 0;
    uint32_t TxMaibox = CAN_TX_MAILBOX0;

    g_canx_txheader.StdId = id; /* 标准ID */
    g_canx_txheader.ExtId = id; /* 扩展ID(29位) */
    g_canx_txheader.IDE = CAN_ID_STD; /* 标准帧 */
    g_canx_txheader.RTR = CAN_RTR_DATA; /* 数据帧 */
    g_canx_txheader.DLC = len; /* 数据长度 */

    if(HAL_CAN_AddTxMessage(&g_canx_handler, &g_canx_txheader, msg, &TxMaibox) != HAL_OK) /* 发送CAN消息 */
    {
        return 1;
    }
    while(HAL_CAN_GetTxMailboxesFreeLevel(&g_canx_handler) != 3) /* 等待发送完成 ,所有邮箱都空闲 */
    {
        t++;

        if(t > 0xFFF)
        {
            HAL_CAN_AbortTxRequest(&g_canx_handler, TxMaibox); /* 超时，直接中止邮箱的发送请求 */
            return 1;
        }
    }
    return 0;
 }
 /**
 * @brief       CAN 接收数据查询
 *   @note      接收数据格式固定为: 标准ID, 数据帧
 * @param       id      : 要查询的 标准ID(11位)
 * @param       buf     : 数据缓存区
 * @retval      接收结果
 *   @arg       0   , 无数据被接收到;
 *   @arg       其他, 接收的数据长度
 */
 uint8_t can_receive_msg(uint32_t id, uint8_t *buf)
 {
    if(HAL_CAN_GetRxFifoFillLevel(&g_canx_handler, CAN_RX_FIFO0) == 0) /*没有数据被接收到*/
    {
        return 0;
    }

    if(HAL_CAN_GetRxMessage(&g_canx_handler, CAN_RX_FIFO0, &g_canx_rxheader, buf) != HAL_OK) /* 接收CAN消息读取数据 */
    {
        return 0;
    }

    if (g_canx_rxheader.StdId!= id || g_canx_rxheader.IDE != CAN_ID_STD || g_canx_rxheader.RTR != CAN_RTR_DATA)       /* 接收到的ID不对 / 不是标准帧 / 不是数据帧 */
    {
        return 0;
    }
    return g_canx_rxheader.DLC;
 }