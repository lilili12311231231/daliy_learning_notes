#include "./BSP/OLED/oled.h"

static void oled_wr_byte(uint8_t data, uint8_t cmd);
static void oled_data_out(uint8_t data);

#define OLED_MODE 1 /* OLED模式 */
#define OLED_CMD 0 /* OLED命令 */
#define OLED_DATA 1 /* OLED数据 */

void oled_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟 
    __HAL_RCC_GPIOD_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟 
    __HAL_RCC_GPIOE_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟 
    __HAL_RCC_GPIOF_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟 
    __HAL_RCC_GPIOG_CLK_ENABLE();   // 初始化OLED相关IO口, 并使能时钟 
    /* PA4 设置 */
    gpio_init_struct.Pin = GPIO_PIN_4;/* PA4引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);       /* 初始化PA4引脚 */
    /* PB6, PB7 设置 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;/* PB6, PB7引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);       /* 初始化PB6, PB7引脚 */
    /* PC6~9, PC11 设置*/
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;/* PC6~9, PC11引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);       /* 初始化PC6~9, PC11引脚 */
    /* PD6, PD7 设置 */
    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;/* PD6, PD7引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);       /* 初始化PD6, PD7引脚 */
    /* PE5, PE6 设置 */
    gpio_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_6;/* PE5, PE6引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);       /* 初始化PE5, PE6引脚 */
    /* PG15 设置 */
    gpio_init_struct.Pin = GPIO_PIN_15;/* PG15引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);       /* 初始化PG15引脚 */

    OLED_WR(1);/* 关闭 OLED 数据写入 */
    OLED_RD(1);/* 关闭 OLED 数据读取 */ 

    OLED_CS(1);/* 关闭 OLED 选中 */
    OLED_RS(1);/* 关闭 OLED 数据写入 */

    OLED_RST(0);/* 关闭 OLED 重置 */
    delay_ms(100);/* 延时 100ms */
    OLED_RST(1);/* 关闭 OLED 重置 */

    oled_wr_byte(0xAE, OLED_CMD);/* 关闭 OLED 显示 */
    oled_wr_byte(0xD5, OLED_CMD);/* 设置时钟分频因子,震荡频率 */
    oled_wr_byte(0x80, OLED_CMD);/*  [3:0],分频因子;[7:4],震荡频率*/
    oled_wr_byte(0xA8, OLED_CMD);/* 设置驱动路数*/
    oled_wr_byte(0X3F, OLED_CMD); /* 默认 0X3F(1/64) */
    oled_wr_byte(0xD3, OLED_CMD); /* 设置显示偏移 */
    oled_wr_byte(0X00, OLED_CMD); /* 默认为 0 */
    oled_wr_byte(0x40, OLED_CMD); /* 设置显示开始行 [5:0],行数. */
    oled_wr_byte(0x8D, OLED_CMD); /* 电荷泵设置 */
    oled_wr_byte(0x14, OLED_CMD); /* bit2，开启/关闭 */
    oled_wr_byte(0x20, OLED_CMD); /* 设置内存地址模式 */
    /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认 10; */
    oled_wr_byte(0x02, OLED_CMD);
    oled_wr_byte(0xA1, OLED_CMD); /* 段重定义设置,bit0:0,0->0;1,0->127; */
    /* 设置 COM 扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */ 
    oled_wr_byte(0xC8, OLED_CMD); 
    oled_wr_byte(0xDA, OLED_CMD); /* 设置 COM 硬件引脚配置 */
    oled_wr_byte(0x12, OLED_CMD); /* [5:4]配置 */
    oled_wr_byte(0x81, OLED_CMD); /* 对比度设置 */
    oled_wr_byte(0xEF, OLED_CMD); /* 1~255;默认 0X7F (亮度设置,越大越亮) */
    oled_wr_byte(0xD9, OLED_CMD); /* 设置预充电周期 */
    oled_wr_byte(0xf1, OLED_CMD); /* [3:0],PHASE 1;[7:4],PHASE 2; */
    oled_wr_byte(0xDB, OLED_CMD); /* 设置 VCOMH 电压倍率 */
    /*[6:4]000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */ 
    oled_wr_byte(0x30, OLED_CMD);
    oled_wr_byte(0xA4, OLED_CMD); /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
    oled_wr_byte(0xA6, OLED_CMD); /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
    oled_wr_byte(0xAF, OLED_CMD); /* 开启显示 */
    oled_clear();
}
/* 
* OLED 的显存
* 每个字节表示 8 个像素, 128,表示有 128 列, 8 表示有 64 行, 高位表示高行数. 
* 比如:g_oled_gram[0][0],包含了第一列,第 1~8 行的数据. g_oled_gram[0][0].0,即表示坐标
* (0,0)
* 类似的: g_oled_gram[1][0].1,表示坐标(1,1), g_oled_gram[10][1].2,表示坐标(10,10),
* 存放格式如下(高位表示高行数).
* [0]0 1 2 3 ... 127
* [1]0 1 2 3 ... 127
* [2]0 1 2 3 ... 127
* [3]0 1 2 3 ... 127
* [4]0 1 2 3 ... 127
* [5]0 1 2 3 ... 127
* [6]0 1 2 3 ... 127
* [7]0 1 2 3 ... 127
*/
static uint8_t g_oled_gram[128][8];
void oled_refresh_gram(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        oled_wr_byte (0xb0 + i, OLED_CMD); /* 设置页地址（0~7） */
        oled_wr_byte (0x00, OLED_CMD); /* 设置显示位置—列低地址 */
        oled_wr_byte (0x10, OLED_CMD); /* 设置显示位置—列高地址 */
        for (n = 0; n < 128; n++)
            {
            oled_wr_byte(g_oled_gram[n][i], OLED_DATA);
            }
    }
}
/**
* @brief 向 OLED 写入一个字节
* @param data: 要输出的数据
* @param cmd: 数据/命令标志 0,表示命令;1,表示数据;
* @retval 无
*/
static void oled_wr_byte(uint8_t data, uint8_t cmd)
{
    oled_data_out(data);
    OLED_RS(cmd);
    OLED_CS(0);
    OLED_WR(0);
    OLED_WR(1);
    OLED_CS(1);
    OLED_RS(1);
}
/**
* @brief 通过拼凑的方法向 OLED 输出一个 8 位数据
* @param data: 要输出的数据
* @retval 无
*/
static void oled_data_out(uint8_t data)
{
    uint16_t dat = data & 0X0F;
    GPIOC->ODR &= ~(0XF << 6); /* 清空 6~9 */
    GPIOC->ODR |= dat << 6; /* D[3:0]-->PC[9:6] */
    GPIOC->ODR &= ~(0X1 << 11); /* 清空 11 */
    GPIOC->ODR |= ((data >> 4) & 0x01) << 11;
    GPIOB->ODR &= ~(0X1 << 6); /* 清空 6 */
    GPIOB->ODR |= ((data >> 5) & 0x01) << 6;
    GPIOE->ODR &= ~(0X3 << 5); /* 清空 5,6 */
    GPIOE->ODR |= ((data >> 6) & 0x01) << 5;
    GPIOE->ODR |= ((data >> 7) & 0x01) << 6;
}

/**
* @brief OLED 画点
* @param x : 0~127
* @param y : 0~63
* @param dot: 1 填充 0,清空
* @retval 无
*/
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
    uint8_t pos, bx, temp = 0;
    if (x > 127 || y > 63) return; /* 超出范围了 */
    
    pos = y / 8; /* 计算 GRAM 里面的 y 坐标所在的字节, 每个字节可以存储 8 个行坐标 */
    
    bx = y % 8; /* 取余数,方便计算 y 在对应字节里面的位置,及行(y)位置 */
    temp = 1 << bx; /* 高位表示高行号, 得到 y 对应的 bit 位置,将该 bit 先置 1 */
    if (dot) /* 画实心点 */
    {
        g_oled_gram[x][pos] |= temp;
    }
    else /* 画空点,即不显示 */
    {
        g_oled_gram[x][pos] &= ~temp;
    }
}
/**
* @brief 在指定位置显示一个字符,包括部分字符
* @param x : 0~127
* @param y : 0~63
* @param size: 选择字体 12/16/24
* @param mode: 0,反白显示;1,正常显示
* @retval 无
*/
void oled_show_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t *pfont = 0;
    /* 得到字体一个字符对应点阵集所占的字节数 */
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);
    chr = chr - ' '; /* 得到偏移后的值,因为字库是从空格开始存储的,第一个字符是空格 */
    if (size == 12) /* 调用 1206 字体 */
    {
        pfont = (uint8_t *)oled_asc2_1206[chr]; 
    }
    else if (size == 16) /* 调用 1608 字体 */
    {
        pfont = (uint8_t *)oled_asc2_1608[chr];
    }
    else if (size == 24) /* 调用 2412 字体 */
    {
        pfont = (uint8_t *)oled_asc2_2412[chr];
    }
    else /* 没有的字库 */
    {
        return; 
    }
    
    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)oled_draw_point(x, y, mode);
            else oled_draw_point(x, y, !mode);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
/**
   * @brief  OLED 清屏
   */
  void oled_clear(void)
  {
      uint8_t i, n;
      for (i = 0; i < 8; i++)
      {
          for (n = 0; n < 128; n++)
          {
              g_oled_gram[n][i] = 0X00;
          }
      }
      oled_refresh_gram();    /* 更新显示 */
  }

  /**
   * @brief  在指定位置显示一个字符串
   * @param  x,y : 起始坐标(0~127 / 0~63)
   * @param  str : 字符串首地址
   * @param  size: 字体大小 12/16/24
   */
  void oled_show_string(uint8_t x, uint8_t y, uint8_t *str, uint8_t size)
  {
      uint8_t x0 = x;
      uint8_t half_word = size / 2;   /* 一个字符占半宽 */
      while ((*str <= '~') && (*str >= ' '))  /* 只显示可打印 ASCII */
      {
          if (x > (127 - half_word))  /* 超宽换行 */
          {
              x = x0;
              y += size;
          }
          if (y > (63 - size))        /* 超高退出 */
          {
              break;
          }
          oled_show_char(x, y, *str, size, 1);
          x += half_word;
          str++;
      }
  }

  /**
   * @brief  m^n（oled_show_num 的辅助函数）
   */
  static uint32_t oled_pow(uint8_t m, uint8_t n)
  {
      uint32_t result = 1;
      while (n--) result *= m;
      return result;
  }

  /**
   * @brief  在指定位置显示一个数字（固定长度）
   * @param  x,y : 起始坐标
   * @param  num : 要显示的数字
   * @param  len : 显示位数
   * @param  size: 字体大小 12/16/24
   */
  void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
  {
      uint8_t t, temp;
      uint8_t enshow = 0;
      for (t = 0; t < len; t++)
      {
          temp = (num / oled_pow(10, len - t - 1)) % 10;
          if (enshow == 0 && t < (len - 1))       /* 前导零处理 */
          {
              if (temp == 0)
              {
                  oled_show_char(x + (size / 2) * t, y, ' ', size, 1);
                  continue;
              }
              else
              {
                  enshow = 1;
              }
          }
          oled_show_char(x + (size / 2) * t, y, temp + '0', size, 1);
      }
  }