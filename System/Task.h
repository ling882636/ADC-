#ifndef __TASK_H
#define __TASK_H

#include <stdint.h>

typedef struct {
    uint16_t raw_data;
    uint16_t filtered_data;
    uint8_t filter_enabled; // 1 enable filter, 0 disable
    float alpha;           // low-pass filter weight
    uint16_t adc_pause_counter; // pause ADC output after CLI command
    uint8_t command_processed;  // flag to skip ADC after command
} TaskData_t;

extern TaskData_t task_data;

void Task_Run(void);
void Task_ADC_Process(void);
void Task_CLI_Process(void);

#endif
