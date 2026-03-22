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

        if (data_ready)
        {
            data_ready = 0;
			uint16_t v = voltage * 100;
			Serial_Printf("%d,%.2f\n", adc_value, voltage);
			OLED_ShowString(1, 1, "V:");
			OLED_ShowNum(1, 3, v / 100, 1);  // 整数部分
			OLED_ShowChar(1, 4, '.');
			OLED_ShowNum(1, 5, v % 100, 2);  // 小数部分
            OLED_DrawWave(adc_value);
        }
    }
}
