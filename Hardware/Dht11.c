#include "stm32f10x.h"
#include "Dht11.h"
#include "Delay.h"
#include <stddef.h>

/* 设置数据线为输出模式 */
static void DHT11_Mode_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/* 设置数据线为输入模式 */
static void DHT11_Mode_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;  /* 上拉输入 */
    GPIO_InitStructure.GPIO_Pin   = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/* 读取数据线电平 */
static uint8_t DHT11_Read_Data(void)
{
    return GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

/**
  * @brief  DHT11 初始化
  */
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);
    DHT11_Mode_Out();
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);   /* 默认拉高 */
    Delay_ms(100);                                    /* 上电后等待 DHT11 稳定 */
}

/**
  * @brief  读取 DHT11 的一个字节
  * @param  无
  * @retval 读取的字节数据
  */
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t byte = 0;
    uint8_t i, j;

    for (i = 0; i < 8; i++)
    {
        /* 等待低电平结束 (每位开始前约 50us 低电平)，加超时防止死循环 */
        j = 0;
        while (DHT11_Read_Data() == 0) { if (++j > 200) return 0; }
        Delay_us(35);   /* 延时 35us 后判断：0 的高电平 26-28us，1 的高电平约 70us */
        byte <<= 1;
        if (DHT11_Read_Data() == 1)
        {
            byte |= 1;
        }
        /* 等待高电平结束，加超时防止死循环 */
        j = 0;
        while (DHT11_Read_Data() == 1) { if (++j > 200) return 0; }
    }
    return byte;
}

/**
  * @brief  读取 DHT11 温湿度数据
  */
uint8_t DHT11_Read(DHT11_Data_TypeDef *data)
{
    uint8_t buf[5];
    uint8_t i;

    if (data == NULL)
    {
        return 1;
    }

    /* 主机发送起始信号：拉低至少 18ms，再拉高 20-40us */
    DHT11_Mode_Out();
    GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    Delay_ms(20);
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    Delay_us(30);

    /* 切换为输入，等待 DHT11 响应 */
    DHT11_Mode_In();

    /* 等待 DHT11 拉低 (响应信号 ~80us) */
    i = 0;
    while (DHT11_Read_Data() == 1)
    {
        Delay_us(1);
        if (++i > 100) return 1;   /* 超时 */
    }

    /* 等待 DHT11 拉高 (~80us) */
    i = 0;
    while (DHT11_Read_Data() == 0)
    {
        Delay_us(1);
        if (++i > 100) return 1;
    }

    /* 等待 DHT11 再次拉低 (开始发送数据) */
    i = 0;
    while (DHT11_Read_Data() == 1)
    {
        Delay_us(1);
        if (++i > 100) return 1;
    }

    /* 读取 40 位数据 */
    for (i = 0; i < 5; i++)
    {
        buf[i] = DHT11_Read_Byte();
    }

    /* 校验 */
    if (buf[4] != (buf[0] + buf[1] + buf[2] + buf[3]))
    {
        return 1;   /* 校验失败 */
    }

    data->humidity_int = buf[0];
    data->humidity_dec = buf[1];
    data->temp_int     = buf[2];
    data->temp_dec     = buf[3];
    data->checksum     = buf[4];

    return 0;   /* 成功 */
}
