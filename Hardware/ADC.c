#include "stm32f10x.h"

void ADC1_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 |
                            RCC_APB2Periph_GPIOA, ENABLE);

    /* GPIO配置 */
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ADC配置 */
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    /* 校准 */
    ADC_ResetCalibration(ADC1);
    {
        uint32_t timeout = 0xFFFF;
        while (ADC_GetResetCalibrationStatus(ADC1) && timeout--);
    }
    ADC_StartCalibration(ADC1);
    {
        uint32_t timeout = 0xFFFF;
        while (ADC_GetCalibrationStatus(ADC1) && timeout--);
    }
}

uint16_t ADC_Read(void)
{
    ADC_RegularChannelConfig(ADC1,
                             ADC_Channel_0,
                             1,
                             ADC_SampleTime_55Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    uint32_t timeout = 0xFFFF;
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) && timeout--) ;

    return ADC_GetConversionValue(ADC1);
}


