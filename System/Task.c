#include "stm32f10x.h"
#include "stdio.h"
#include "ADC.h"
#include "Filter.h"
#include "SysTick.h"
#include "Task.h"
#include "CLI.h"

TaskData_t task_data = {
    .raw_data = 0,
    .filtered_data = 0,
    .filter_enabled = 1,
    .alpha = 0.1f,
    .adc_pause_counter = 0,
    .command_processed = 0,
};

void Task_ADC_Process(void)
{
    if(dma_half_flag)
    {
        dma_half_flag = 0;

        for(int i = 0; i < ADC_BUF_SIZE/2; i++)
        {
            uint16_t raw = adc_buffer[i];
            uint16_t filtered = task_data.filter_enabled ? low_pass_filter(raw) : raw;

            task_data.raw_data = raw;
            task_data.filtered_data = filtered;
        }
    }

    if(dma_full_flag)
    {
        dma_full_flag = 0;

        for(int i = ADC_BUF_SIZE/2; i < ADC_BUF_SIZE; i++)
        {
            uint16_t raw = adc_buffer[i];
            uint16_t filtered = task_data.filter_enabled ? low_pass_filter(raw) : raw;

            task_data.raw_data = raw;
            task_data.filtered_data = filtered;
        }
    }

    static uint32_t last_tick = 0;
    if(sys_tick - last_tick >= 50)
    {
        last_tick = sys_tick;
        if (task_data.adc_pause_counter == 0)
        {
            printf("%d, %d\n", task_data.raw_data, task_data.filtered_data);
        }
        else if (task_data.adc_pause_counter != 0xFFFF)
        {
            task_data.adc_pause_counter--;
        }
    }
}


void Task_CLI_Process(void)
{
    CLI_Process();
}


void Task_Run(void)
{
    Task_CLI_Process();
    if (!task_data.command_processed) {
        Task_ADC_Process();
    }
    task_data.command_processed = 0;
}
