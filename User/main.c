#include "stm32f10x.h"
#include "stdio.h"
#include "ADC.h"
#include "Serial.h"
#include "SysTick.h"
#include "Task.h"

int main(void)
{
    ADC1_Init();
    Serial_Init();
    Serial_SendString("USART1 OK\r\n");
    SysTick_Init();
    MY_DMA_Init();
    DMA_NVIC_Init();

    while(1)
    {
        Task_Run();
    }
}
