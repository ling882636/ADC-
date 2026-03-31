#include "stm32f10x.h"
#include "stdio.h"
#include "ADC.h"
#include "Serial.h"
#include "systick.h"
#include "Filter.h"

uint32_t last_tick = 0;
void process_adc_data(void)
{
    uint16_t filtered_data = low_pass_filter(adc_buffer[0]);

    // 你可以在这里对 `filtered_data` 进行其他处理
    printf("Filtered Value: %d\n", filtered_data);
}

int main(void)
{
    ADC1_Init();
    Serial_Init();
    MY_DMA_Init();
    DMA_NVIC_Init();
    SysTick_Init();  

    while(1)
    {
        // -------- 半缓冲 --------
        if (dma_half_flag)
        {
            dma_half_flag = 0;

            // 处理半缓冲数据
			if(sys_tick - last_tick >= 50)
            {	
				process_adc_data();
			}
        }

        // -------- 全缓冲 --------
        if (dma_full_flag)
        {
            dma_full_flag = 0;

            // 处理全缓冲数据（如果需要）
        }
    }
}
