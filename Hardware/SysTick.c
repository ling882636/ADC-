#include "stm32f10x.h"                  // Device header

volatile uint32_t sys_tick;

void SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / 1000); // 1ms中断
}
