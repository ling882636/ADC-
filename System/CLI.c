#include "cli.h"
#include "Serial.h"
#include "LED.h"
#include "Dht11.h"
#include "Task.h"
#include "stm32f10x.h"
#include "Filter.h"
#include <string.h>
#include <stdio.h>

/* 命令函数声明 */

static void CLI_Help(void);
static void CLI_LED_ON(void);
static void CLI_LED_OFF(void);
static void CLI_DHT(void);
static void CLI_Reboot(void);
static void CLI_FilterOn(void);
static void CLI_FilterOff(void);
static void CLI_AdcOn(void);
static void CLI_AdcOff(void);
/* 命令表 */

typedef struct
{
    char *name;
    void (*func)(void);
} CLI_Command;

CLI_Command cmd_table[] =
{
    {"help",      CLI_Help},
    {"led on",    CLI_LED_ON},
    {"led off",   CLI_LED_OFF},
    {"dht",       CLI_DHT},
    {"reboot",    CLI_Reboot},
    {"filter on", CLI_FilterOn},
    {"filter off",CLI_FilterOff},
    {"adc on",    CLI_AdcOn},
    {"adc off",   CLI_AdcOff}
};

#define CMD_NUM (sizeof(cmd_table)/sizeof(cmd_table[0]))

/* CLI解析 */

void CLI_Process(void)
{
    if(cmd_flag == 0)
        return;

    cmd_flag = 0;

    // 标记命令已处理，跳过本次ADC输出
    task_data.command_processed = 1;

    // 暂停ADC输出5秒（50ms * 100）
    task_data.adc_pause_counter = 100;

    if(strncmp((char*)rx_buffer, "set alpha ", 10) == 0)
    {
        float alpha = 0.0f;
        if(sscanf((char*)rx_buffer + 10, "%f", &alpha) == 1)
        {
            Filter_SetAlpha(alpha);
            task_data.alpha = Filter_GetAlpha();
            char buf[64];
            sprintf(buf, "Alpha set to %.2f\r\n", task_data.alpha);
            Serial_SendString(buf);
        }
        else
        {
            Serial_SendString("Invalid alpha value\r\n");
        }

        memset((char*)rx_buffer,0,RX_BUFFER_SIZE);
        Serial_SendString("\r\nSTM32> ");
        return;
    }

    for(int i=0;i<CMD_NUM;i++)
    {
        if(strcmp((char*)rx_buffer,cmd_table[i].name) == 0)
        {
            cmd_table[i].func();

            memset((char*)rx_buffer,0,RX_BUFFER_SIZE);

            Serial_SendString("\r\nSTM32> ");
            return;
        }
    }

    Serial_SendString("Unknown Command\r\n");
    Serial_SendString("STM32> ");

    memset((char*)rx_buffer,0,RX_BUFFER_SIZE);
}

/* 命令实现 */

static void CLI_Help(void)
{
    Serial_SendString("\r\nAvailable Commands:\r\n");
    Serial_SendString("help      - show command list\r\n");
    Serial_SendString("led on    - turn LED on\r\n");
    Serial_SendString("led off   - turn LED off\r\n");
    Serial_SendString("dht       - read DHT11\r\n");
    Serial_SendString("reboot    - system reset\r\n");
    Serial_SendString("filter on  - enable ADC filtering\r\n");
    Serial_SendString("filter off - disable ADC filtering\r\n");
    Serial_SendString("set alpha x - set low-pass alpha (0.01..0.99)\r\n");
    Serial_SendString("adc on     - enable ADC output\r\n");
    Serial_SendString("adc off    - disable ADC output\r\n");
}

static void CLI_FilterOn(void)
{
    task_data.filter_enabled = 1;
    Serial_SendString("Filter enabled\r\n");
}

static void CLI_FilterOff(void)
{
    task_data.filter_enabled = 0;
    Serial_SendString("Filter disabled\r\n");
}

static void CLI_AdcOn(void)
{
    task_data.adc_pause_counter = 0;
    Serial_SendString("ADC output enabled\r\n");
}

static void CLI_AdcOff(void)
{
    task_data.adc_pause_counter = 0xFFFF; // 永久暂停
    Serial_SendString("ADC output disabled\r\n");
}

void CLI_LED_ON(void)
{
    LED1_ON();
    Serial_SendString("LED ON OK\r\n");
}

void CLI_LED_OFF(void)
{
    LED1_OFF();
    Serial_SendString("LED OFF OK\r\n");
}

void CLI_Info(void)
{
    Serial_SendString("\r\nSystem Info:\r\n");
    Serial_SendString("Board: STM32F103\r\n");
    Serial_SendString("CLI Version: 1.0\r\n");
}

static void CLI_DHT(void)
{
    DHT11_Data_TypeDef data;

    if (DHT11_Read(&data) != 0)
    {
        Serial_SendString("DHT11 read failed\r\n");
        return;
    }

    char buf[64];
    sprintf(buf, "Temp:%dC Hum:%d%%\r\n", data.temp_int, data.humidity_int);
    Serial_SendString(buf);
}

static void CLI_Reboot(void)
{
    Serial_SendString("System Reboot...\r\n");

    NVIC_SystemReset();
}
