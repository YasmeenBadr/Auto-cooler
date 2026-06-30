#ifndef KEYPAD_H
#define KEYPAD_H

#include "Std_Types.h"
#include "Gpio.h"

#define KEYPAD_ROW_0  GPIO_B, 0
#define KEYPAD_ROW_1  GPIO_B, 1
#define KEYPAD_ROW_2  GPIO_B, 2
#define KEYPAD_ROW_3  GPIO_B, 3

#define KEYPAD_COL_0  GPIO_B, 4
#define KEYPAD_COL_1  GPIO_B, 5
#define KEYPAD_COL_2  GPIO_B, 6
#define KEYPAD_COL_3  GPIO_B, 7

#define KEYPAD_NO_PRESSED_KEY 0xFF

void Keypad_Init(void);
uint8 Keypad_GetPressedKey(void);

#endif // KEYPAD_H
