#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

/* DHT11 引脚定义 - 使用 PA0 */
#define DHT11_GPIO_PORT      GPIOA
#define DHT11_GPIO_PIN       GPIO_Pin_0
#define DHT11_GPIO_CLK       RCC_APB2Periph_GPIOA

/* DHT11 数据结构 */
typedef struct
{
    uint8_t  humidity_int;   /* 湿度整数部分 (0-99 %RH) */
    uint8_t  humidity_dec;   /* 湿度小数部分 (DHT11 恒为 0) */
    uint8_t  temp_int;       /* 温度整数部分 (-20~60 ℃) */
    uint8_t  temp_dec;       /* 温度小数部分 (DHT11 恒为 0) */
    uint8_t  checksum;       /* 校验和 */
} DHT11_Data_TypeDef;

/**
  * @brief  DHT11 初始化
  * @param  无
  * @retval 无
  */
void DHT11_Init(void);

/**
  * @brief  读取 DHT11 温湿度数据
  * @param  data: 指向 DHT11_Data_TypeDef 结构体的指针，用于存储读取的数据
  * @retval 0-成功, 1-失败
  */
uint8_t DHT11_Read(DHT11_Data_TypeDef *data);

#endif
