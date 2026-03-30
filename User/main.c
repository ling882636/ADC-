#include "stm32f10x.h"
#include "stdio.h"
#include "ADC.h"
#include "Serial.h"
#include "systick.h"

static uint32_t last_tick = 0;
static uint16_t max_half_flag = 0;
int main(void)
{
    ADC1_Init();
    Serial_Init();
    MY_DMA_Init();
    DMA_NVIC_Init();
    SysTick_Init();  

while(1)
{
    // ✅ 先观测（关键）
    if(dma_half_flag > max_half_flag)
        max_half_flag = dma_half_flag;

    // -------- 半缓冲 --------
    if(dma_half_flag)
    {
        dma_half_flag--;

        if(sys_tick - last_tick >= 10)
        {
            last_tick = sys_tick;
            printf("%d\n", adc_buffer[0]);
        }
    }

    // -------- 全缓冲 --------
    if(dma_full_flag)
    {
        dma_full_flag--;
    }
}
}
