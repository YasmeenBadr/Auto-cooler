#include "Std_Types.h"
#include "Rcc.h"
#include "Gpio.h"
#include "Adc.h"
#include "Pwm.h"
#include "Timer.h"
#include "Lcd1602.h"


typedef enum {
    STATE_IDLE,
    STATE_COOLING,
    STATE_OVERHEAT
} SystemState_t;

volatile uint16 G_ADC_Value = 0; // da l bykhzn value l ADC 

/* ADC Callback: Executed without polling by the NVIC on EOC */
// de l httnfz lma l ADC ykhls conversion
void AdcCallback(uint16 Result) {
    G_ADC_Value = Result;
    Adc_StartConversion();
}

/* Helper to provide a rough block delay for LCD readability */
void DelayMs(uint32 ms) {
    for (uint32 i = 0; i < (ms * 2000); i++) {
        __asm volatile("nop");
    }
}

/* Custom int to string for LCD to avoid heavy stdio.h */
void IntToStr(uint32 num, char* str, uint8 width, char pad) {
    uint8 i = 0;
    if (num == 0) {
        str[i++] = '0';
    } else {
        while (num > 0 && i < 10) {
            str[i++] = 
            (num % 10) + '0';
            num /= 10;
        }
    }
    while (i < width) {
        str[i++] = pad;
    }
    str[i] = '\0';
    // string is reversed, need to flip it
    for (uint8 j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

int main(void) {
    /* 1. Initialization Phase */
    // hnshghl l clock l kol peripheral
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_TIM3);
    Rcc_Enable(RCC_ADC1);

    /* Initialize LCD */
    Lcd1602_Init();
    Lcd1602_Clear();

    /* GPIO Setup */
    Gpio_Init(GPIO_C, 13, GPIO_OUTPUT, GPIO_PUSH_PULL); // Alarm LED
    Gpio_WritePin(GPIO_C, 13, LOW);

    /* Motor PWM Setup (PA6 -> TIM3_CH1) */
    Gpio_Init(GPIO_A, 6, GPIO_AF, GPIO_PUSH_PULL);
    Gpio_SetAlternateFunction(GPIO_A, 6, 2); // AF2 mapping for TIM3
    /* Timer3 Init: F_CPU=16MHz. Target PWM Freq = 100kHz.
       Prescaler=160 -> 1MHz timer clock. AutoReload=2000 -> 100kHz PWM frequency. */
    Pwm_Init(TIMER3, CH1, 159, 1999);
    Pwm_SetDutyPercent(TIMER3, CH1, 0);
    Pwm_Start(TIMER3, CH1);

    /* ADC Setup (PA0 -> ADC1_IN0) */
    Gpio_Init(GPIO_A, 0, GPIO_ANALOG, GPIO_PUSH_PULL); // ADC pin
    Adc_Init(ADC_RES_12BIT);
    Adc_ConfigSingleChannel_OneShot(ADC_CHANNEL_0);
    Adc_ReadSingleChannelAsync(AdcCallback);
    Adc_StartConversion();

    SystemState_t currentState = STATE_IDLE;
    char tempStr[10];
    char fanStr[10];
    uint32 tempCelsius = 0;
    uint8 fanDuty = 0;

    /* 2. Main Logic Loop */
    while (1) {
        /* Convert ADC 12-bit (0-4095) to Temp (LM35 is 10mV/C)
           Based on the datasheet, VDDA should be 2.4V to 3.6V (typically 3.3V):
           Temp = (ADC * 330) / 4095 */
        uint32 adcReading = G_ADC_Value;
        tempCelsius = (adcReading * 330) / 4095;

        /* Evaluate Next State and Output mapping */
        switch (currentState) {
            case STATE_IDLE:
                fanDuty = 0;
                Gpio_WritePin(GPIO_C, 13, LOW);

                if (tempCelsius >= 40) {
                    currentState = STATE_OVERHEAT;
                } else if (tempCelsius >= 25) {
                    currentState = STATE_COOLING;
                }
                break;

            case STATE_COOLING:
                Gpio_WritePin(GPIO_C, 13, LOW);
                
                if (tempCelsius >= 40) {
                    currentState = STATE_OVERHEAT;
                } else if (tempCelsius < 25) {
                    currentState = STATE_IDLE;
                    fanDuty = 0;
                } else if (tempCelsius < 30) {
                    fanDuty = 33;
                } else if (tempCelsius < 35) {
                    fanDuty = 66;
                } else {
                    fanDuty = 100;
                }
                break;

            case STATE_OVERHEAT:
                fanDuty = 100;
                Gpio_WritePin(GPIO_C, 13, HIGH); // Assert Alarm LED

                if (tempCelsius < 40) {
                    currentState = STATE_COOLING;
                    Gpio_WritePin(GPIO_C, 13, LOW);
                }
                break;
            default:
                currentState = STATE_IDLE;
                break;
        }

        /* Actuate Output */
        Pwm_SetDutyPercent(TIMER3, CH1, fanDuty);

        /* Update LCD UI (limit refresh rate to avoid flicker) */
        Lcd1602_SetCursor(0, 0);
        Lcd1602_WriteString("Temp: ");
        IntToStr(tempCelsius, tempStr, 0, ' ');
        Lcd1602_WriteString(tempStr);
        Lcd1602_WriteString(" C     ");

        Lcd1602_SetCursor(1, 0);
        if (currentState == STATE_OVERHEAT) {
            Lcd1602_WriteString("OVERHEAT!       ");
        } else {
            Lcd1602_WriteString("Fan: ");
            IntToStr(fanDuty, fanStr, 0, ' ');
            Lcd1602_WriteString(fanStr);
            Lcd1602_WriteString("%        ");
        }
        /* Drop delay to 30ms so LCD reacts instantly to temperature changes */
        DelayMs(30);
      
    }
}
