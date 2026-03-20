#include "cli.h"
#include "Serial.h"
#include "LED.h"
#include "Dht11.h"
#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
/* 命令函数声明 */

static void CLI_Help(void);
static void CLI_LED_ON(void);
static void CLI_LED_OFF(void);
static void CLI_DHT(void);
static void CLI_Reboot(void);
/* 命令表 */

typedef struct
{
    char *name;
    void (*func)(void);
} CLI_Command;

CLI_Command cmd_table[] =
{
    {"help",    CLI_Help},
    {"led on",  CLI_LED_ON},
    {"led off", CLI_LED_OFF},
    {"dht",     CLI_DHT},
    {"reboot",  CLI_Reboot}
};

#define CMD_NUM (sizeof(cmd_table)/sizeof(cmd_table[0]))

/* CLI解析 */

void CLI_Process(void)
{
    if(cmd_flag == 0)
        return;

    cmd_flag = 0;

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
