#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define ADC_BUF_SIZE 100

// 外部变量声明（给main.c、中断用）
extern uint16_t adc_buffer[ADC_BUF_SIZE];
extern volatile uint16_t dma_half_flag;
extern volatile uint16_t dma_full_flag;

// 函数声明
void MY_DMA_Init(void);
void DMA_NVIC_Init(void);
void ADC1_Init(void);

#endif
