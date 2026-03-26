#include "stm32f10x.h"
#include "ADC.h"
#include "Serial.h"


int main(void)
{
    ADC1_Init();
    Serial_Init();


while(1)
{
    if(dma_half_flag)
    {
        dma_half_flag = 0;

        for(int i = 0; i < ADC_BUF_SIZE/2; i++)
        {
            Serial_Printf("%d\n", adc_buffer[i]);
        }
    }

    if(dma_full_flag)
    {
        dma_full_flag = 0;

        for(int i = ADC_BUF_SIZE/2; i < ADC_BUF_SIZE; i++)
        {
            Serial_Printf("%d\n", adc_buffer[i]);
        }
    }
}
}
