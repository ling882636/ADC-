#include "stm32f10x.h"
#include "OLED_Font.h"
#include <stddef.h>

/* ================== 波形区域定义 ================== */
#define WAVE_TOP     16
#define WAVE_BOTTOM  63
#define WAVE_HEIGHT  (WAVE_BOTTOM - WAVE_TOP)

/* 引脚 */
#define OLED_W_SCL(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA(x) GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/* 显存 */
static uint8_t OLED_GRAM[8][128];

/* ================== I2C ================== */
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

/* ================== 基础函数 ================== */
void OLED_WriteCommand(uint8_t Command)
{
    __disable_irq();
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
    __enable_irq();
}

void OLED_WriteData(uint8_t Data)
{
    __disable_irq();
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);
    OLED_I2C_SendByte(0x40);
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
    __enable_irq();
}

void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));
    OLED_WriteCommand(0x00 | (X & 0x0F));
}

/* ================== 清屏 ================== */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 128; i++)
        {
            OLED_GRAM[j][i] = 0x00;
        }
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/* 只清波形区 */
void OLED_ClearWave(void)
{
    uint8_t page, i;
    for (page = WAVE_TOP / 8; page <= WAVE_BOTTOM / 8; page++)
    {
        for (i = 0; i < 128; i++)
        {
            OLED_GRAM[page][i] = 0x00;
            OLED_SetCursor(page, i);
            OLED_WriteData(0x00);
        }
    }
}

/* ================== 文字 ================== */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    if (Char < ' ' || Char > '~') Char = ' ';

    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);

    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);
    for (i = 0; i < 8; i++)
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
}

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) Result *= X;
    return Result;
}

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i,
            Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/* ================== 画点 ================== */
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
    uint8_t page, bit;
    if (x >= 128 || y >= 64) return;

    page = y / 8;
    bit = y % 8;

    OLED_GRAM[page][x] |= (1 << bit);
    OLED_SetCursor(page, x);
    OLED_WriteData(OLED_GRAM[page][x]);
}

/* ================== 坐标轴 ================== */
void OLED_DrawAxis(void)
{
    uint8_t i;

    for (i = 0; i < 128; i++)
        OLED_DrawPoint(i, WAVE_BOTTOM);

    for (i = WAVE_TOP; i <= WAVE_BOTTOM; i++)
        OLED_DrawPoint(0, i);
}

/* ================== 波形 ================== */
void OLED_DrawWave(uint16_t adc)
{
    static uint8_t x = 0;
    uint8_t y;

    y = WAVE_BOTTOM - (adc * WAVE_HEIGHT / 4095);

    OLED_DrawPoint(x, y);

    x++;
    if (x >= 128)
    {
        x = 0;
        OLED_ClearWave();
        OLED_DrawAxis();
    }
}

/* ================== 初始化 ================== */
void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++)
        for (j = 0; j < 1000; j++);

    OLED_I2C_Init();

    OLED_WriteCommand(0xAE);
    OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
    OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F);
    OLED_WriteCommand(0xD3); OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x40);
    OLED_WriteCommand(0xA1);
    OLED_WriteCommand(0xC8);
    OLED_WriteCommand(0xDA); OLED_WriteCommand(0x12);
    OLED_WriteCommand(0x81); OLED_WriteCommand(0xCF);
    OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1);
    OLED_WriteCommand(0xDB); OLED_WriteCommand(0x30);
    OLED_WriteCommand(0xA4);
    OLED_WriteCommand(0xA6);
    OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14);
    OLED_WriteCommand(0xAF);

    OLED_Clear();
    OLED_DrawAxis();
}
