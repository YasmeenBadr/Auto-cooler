#include "Keypad.h"

/* --- Keypad Configuration based on Image --- */

// Row Pins (A, B, C, D) -> PB3, PB2, PB1, PB0
static const uint8 Keypad_RowPins[4][2] = {
    {GPIO_B, 3}, {GPIO_B, 2}, {GPIO_B, 1}, {GPIO_B, 0}
};

// Column Pins (1, 2, 3, 4) -> PB4, PB5, PB6, PB7
static const uint8 Keypad_ColPins[4][2] = {
    {GPIO_B, 4}, {GPIO_B, 5}, {GPIO_B, 6}, {GPIO_B, 7}
};

// Key Map following the image layout
static const uint8 Keypad_Keys[4][4] = {
    {'7', '8', '9', '/'}, // Row A (PB3)
    {'4', '5', '6', '*'}, // Row B (PB2)
    {'1', '2', '3', '-'}, // Row C (PB1)
    {'C', '0', '=', '+'}  // Row D (PB0) - 'C' is the ON/C button
};

void Keypad_Init(void) {
    // Initialize Rows as Outputs
    for (uint8 i = 0; i < 4; i++) {
        Gpio_Init(Keypad_RowPins[i][0], Keypad_RowPins[i][1], GPIO_OUTPUT, GPIO_PUSH_PULL);
        // Default rows to HIGH when not scanning
        Gpio_WritePin(Keypad_RowPins[i][0], Keypad_RowPins[i][1], HIGH);
    }
    // Initialize Columns as Inputs with Pull-Up
    for (uint8 i = 0; i < 4; i++) {
        Gpio_Init(Keypad_ColPins[i][0], Keypad_ColPins[i][1], GPIO_INPUT, GPIO_PULL_UP);
    }
}

uint8 Keypad_GetPressedKey(void) {
    uint8 pressedKey = KEYPAD_NO_PRESSED_KEY;
    volatile uint32 delay;

    for (uint8 row = 0; row < 4; row++) {
        // Activate current row (Drive LOW)
        Gpio_WritePin(Keypad_RowPins[row][0], Keypad_RowPins[row][1], LOW);

        for (uint8 col = 0; col < 4; col++) {
            // Check if column is pulled LOW by the active row
            if (Gpio_ReadPin(Keypad_ColPins[col][0], Keypad_ColPins[col][1]) == LOW) {
                
                /* Software Debounce */
                for(delay = 0; delay < 10000; delay++); 
                
                if (Gpio_ReadPin(Keypad_ColPins[col][0], Keypad_ColPins[col][1]) == LOW) {
                    pressedKey = Keypad_Keys[row][col];
                    
                    /* Wait for User to Release Key */
                    while (Gpio_ReadPin(Keypad_ColPins[col][0], Keypad_ColPins[col][1]) == LOW);
                    
                    /* Settling delay after release */
                    for(delay = 0; delay < 5000; delay++); 

                    // Return row to idle state before returning
                    Gpio_WritePin(Keypad_RowPins[row][0], Keypad_RowPins[row][1], HIGH);
                    return pressedKey;
                }
            }
        }
        // Deactivate row (Drive HIGH) before moving to next
        Gpio_WritePin(Keypad_RowPins[row][0], Keypad_RowPins[row][1], HIGH);
    }
    return pressedKey;
}