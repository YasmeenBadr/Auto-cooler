#ifndef LED_BAR_H
#define LED_BAR_H

#include "Std_Types.h"
#include "Gpio.h"

/* Progress LEDs */
#define LED_PROGRESS_1  GPIO_A, 1
#define LED_PROGRESS_2  GPIO_A, 2
#define LED_PROGRESS_3  GPIO_A, 3
#define LED_PROGRESS_4  GPIO_A, 5

/* Status LEDs */
#define LED_SUCCESS     GPIO_A, 6
#define LED_ALARM       GPIO_A, 7
#define LED_BELL        GPIO_A, 8

void LedBar_Init(void);
void LedBar_SetProgress(uint8 progress);
void LedBar_SetSuccess(boolean state);
void LedBar_SetAlarm(boolean state);
void LedBar_ToggleBell(void);

#endif // LED_BAR_H
