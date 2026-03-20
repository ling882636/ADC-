#include "stm32f10x.h"      // Device header
#include <stdio.h>
#include "Serial.h"   
#include "OLED.h"   
#include "ADC.h"   
#include "Tim.h"   
#include "CLI.h"   

volatile uint16_t adc_value;
extern volatile uint8_t data_ready;
extern float voltage;
int main(void)
{
    Serial_Init();
    OLED_Init();
    ADC1_Init();
    Timer_Init();
    CLI_Process();

    while(1)
    {
        CLI_Process();

        if(data_ready)
        {
            data_ready = 0;

            Serial_Printf("%.2f\n", voltage);

            char buf[20];
            sprintf(buf, "V:%.2fV", voltage);
            OLED_ShowString(0, 0, buf);

            OLED_DrawWave(adc_value);
        }
    }
}
