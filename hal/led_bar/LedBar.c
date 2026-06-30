#include "LedBar.h"

static const uint8 ProgressLeds[4][2] = {
    {LED_PROGRESS_1}, {LED_PROGRESS_2}, {LED_PROGRESS_3}, {LED_PROGRESS_4}
};

void LedBar_Init(void) {
    /* Initialize Progress LEDs */
    for (uint8 i = 0; i < 4; i++) {
        Gpio_Init(ProgressLeds[i][0], ProgressLeds[i][1], GPIO_OUTPUT, GPIO_PUSH_PULL);
        Gpio_WritePin(ProgressLeds[i][0], ProgressLeds[i][1], LOW);
    }

    /* Initialize Status LEDs */
    Gpio_Init(LED_SUCCESS, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(LED_SUCCESS, LOW);

    Gpio_Init(LED_ALARM, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(LED_ALARM, LOW);

    Gpio_Init(LED_BELL, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(LED_BELL, LOW);
}

void LedBar_SetProgress(uint8 progress) {
    for (uint8 i = 0; i < 4; i++) {
        if (i < progress) {
            Gpio_WritePin(ProgressLeds[i][0], ProgressLeds[i][1], HIGH);
        } else {
            Gpio_WritePin(ProgressLeds[i][0], ProgressLeds[i][1], LOW);
        }
    }
}

void LedBar_SetSuccess(boolean state) {
    Gpio_WritePin(LED_SUCCESS, state);
}

void LedBar_SetAlarm(boolean state) {
    Gpio_WritePin(LED_ALARM, state);
}

void LedBar_ToggleBell(void) {
    static uint8 bellState = LOW;
    bellState = !bellState;
    Gpio_WritePin(LED_BELL, bellState);
}
