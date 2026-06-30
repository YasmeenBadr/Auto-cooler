#ifndef LCD1602_H
#define LCD1602_H

#include "Std_Types.h"

void Lcd1602_Init(void);
void Lcd1602_Clear(void);
void Lcd1602_SetCursor(uint8 Row, uint8 Col);
void Lcd1602_WriteChar(char Data);
void Lcd1602_WriteString(const char *Text);

#endif
