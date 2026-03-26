#ifndef __ADC_H
#define __ADC_H

#define ADC_BUF_SIZE 100

uint16_t adc_buffer[ADC_BUF_SIZE];

extern volatile uint8_t dma_half_flag = 0;
extern volatile uint8_t dma_full_flag = 0;

void ADC1_Init(void);
uint16_t ADC_Read(void);


#endif
