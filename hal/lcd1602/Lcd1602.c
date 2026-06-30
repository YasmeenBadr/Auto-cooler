#include "Lcd1602.h"

#include "Gpio.h"

#define LCD_PORT GPIO_B
#define LCD_RS_PIN 0U
#define LCD_EN_PIN 1U
#define LCD_D4_PIN 2U
#define LCD_D5_PIN 10U
#define LCD_D6_PIN 12U
#define LCD_D7_PIN 13U

static void Lcd_DelayCycles(uint32 Cycles)
{
    volatile uint32 i;
    for (i = 0; i < Cycles; i++)
    {
        __asm volatile ("nop");
    }
}

static void Lcd_WriteNibble(uint8 Nibble)
{
    Gpio_WritePin(LCD_PORT, LCD_D4_PIN, (Nibble >> 0U) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D5_PIN, (Nibble >> 1U) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D6_PIN, (Nibble >> 2U) & 0x01U);
    Gpio_WritePin(LCD_PORT, LCD_D7_PIN, (Nibble >> 3U) & 0x01U);

    Gpio_WritePin(LCD_PORT, LCD_EN_PIN, HIGH);
    Lcd_DelayCycles(600U);
    Gpio_WritePin(LCD_PORT, LCD_EN_PIN, LOW);
    Lcd_DelayCycles(1200U);
}

static void Lcd_WriteByte(uint8 IsData, uint8 Value)
{
    Gpio_WritePin(LCD_PORT, LCD_RS_PIN, IsData);
    Lcd_WriteNibble((uint8)(Value >> 4U));
    Lcd_WriteNibble((uint8)(Value & 0x0FU));
}

void Lcd1602_Init(void)
{
    Gpio_Init(LCD_PORT, LCD_RS_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_EN_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D4_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D5_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D6_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(LCD_PORT, LCD_D7_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);

    Lcd_DelayCycles(120000U);

    Gpio_WritePin(LCD_PORT, LCD_RS_PIN, LOW);
    Gpio_WritePin(LCD_PORT, LCD_EN_PIN, LOW);

    Lcd_WriteNibble(0x03U);
    Lcd_DelayCycles(40000U);
    Lcd_WriteNibble(0x03U);
    Lcd_DelayCycles(12000U);
    Lcd_WriteNibble(0x03U);
    Lcd_DelayCycles(12000U);
    Lcd_WriteNibble(0x02U);

    Lcd_WriteByte(0U, 0x28U);
    Lcd_WriteByte(0U, 0x0CU);
    Lcd_WriteByte(0U, 0x06U);
    Lcd_WriteByte(0U, 0x01U);
    Lcd_DelayCycles(60000U);
}

void Lcd1602_Clear(void)
{
    Lcd_WriteByte(0U, 0x01U);
    Lcd_DelayCycles(60000U);
}

void Lcd1602_SetCursor(uint8 Row, uint8 Col)
{
    uint8 Address = (uint8)(Col & 0x0FU);

    if (Row == 1U)
    {
        Address = (uint8)(Address + 0x40U);
    }

    Lcd_WriteByte(0U, (uint8)(0x80U | Address));
}

void Lcd1602_WriteChar(char Data)
{
    Lcd_WriteByte(1U, (uint8)Data);
}

void Lcd1602_WriteString(const char *Text)
{
    while ((Text != 0) && (*Text != '\0'))
    {
        Lcd1602_WriteChar(*Text);
        Text++;
    }
}
