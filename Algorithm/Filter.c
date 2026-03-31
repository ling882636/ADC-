#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>
/**
 * @brief  平均值滤波（安全版，防异常输入）
 * @param  buf: 数据缓冲区指针
 * @param  len: 数据长度 (0 < len <= 255)
 * @return 平均值
 */
uint16_t Filter_Average(uint16_t *buf, uint8_t len)
{
    uint32_t sum = 0;

    // 安全判断：空指针 或 长度为0
    if (buf == NULL || len == 0)
    {
        return 0;
    }

    // 无符号循环变量，更适合嵌入式
    for (uint8_t i = 0; i < len; i++)
    {
        sum += buf[i];
    }

    // 四舍五入（比直接截断更精准）
    return (uint16_t)((sum + len / 2) / len);
}

/**
 * @brief  中值滤波（不修改原始数据，安全版）
 * @param  buf: 原始数据数组
 * @param  len: 数据长度（建议奇数：3/5/7/9...）
 * @return 中值
 */
uint16_t Filter_Median(uint16_t *buf, uint8_t len)
{
    // 安全校验
    if (buf == NULL || len < 1)
        return 0;

    // 建临时数组（static 避免栈溢出），边界检查
    if (len > 255) return 0;
    static uint16_t tmp_buf[255];
    memcpy(tmp_buf, buf, len * sizeof(uint16_t));

    uint16_t temp;
    // 冒泡排序（从小到大）
    for (uint8_t i = 0; i < len - 1; i++)
    {
        for (uint8_t j = 0; j < len - i - 1; j++)
        {
            if (tmp_buf[j] > tmp_buf[j + 1])
            {
                temp = tmp_buf[j];
                tmp_buf[j] = tmp_buf[j + 1];
                tmp_buf[j + 1] = temp;
            }
        }
    }

    // 返回中间值
    return tmp_buf[len / 2];
}

#define FILTER_SIZE 10    // 滤波窗口大小：保存最近10个数据

uint16_t Filter_MovingAverage(uint16_t new_value)
{
    static uint16_t buffer[FILTER_SIZE];  // 静态数组：保存历史数据
    static uint8_t index = 0;             // 静态下标：记录当前写到哪里
    uint32_t sum = 0;                     // 求和（用32位防止溢出）

    // 1. 新数据放入数组
    buffer[index++] = new_value;

    // 2. 循环覆盖：到了末尾就回到开头（滑动核心！）
    if(index >= FILTER_SIZE)
        index = 0;

    // 3. 把所有10个数据加起来
    for(int i=0;i<FILTER_SIZE;i++)
    {
        sum += buffer[i];
    }

    // 4. 求平均并返回
    return sum / FILTER_SIZE;
}

#define ALPHA 0.1  // 控制滤波器的平滑程度，0 < ALPHA < 1


uint16_t low_pass_filter(uint16_t input)
{
	static uint16_t last_value = 0;  // 用于存储前一个输出
    last_value = (uint16_t)(ALPHA * input + (1 - ALPHA) * last_value);
    return last_value;
}
