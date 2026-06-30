#include "Exti.h"

typedef struct {
    volatile uint32 IMR;
    volatile uint32 EMR;
    volatile uint32 RTSR;
    volatile uint32 FTSR;
    volatile uint32 SWIER;
    volatile uint32 PR;
} ExtiType;

typedef struct {
    volatile uint32 NVIC_ISER[8];
    uint32 _r[24];
    volatile uint32 NVIC_ICER[8];
} NvicType;

typedef struct {
    uint32 MEMRMP;
    uint32 PMC;
    uint32 EXTICR[4];
    uint32 _r[2];
    uint32 CMPCR;
} SyscfgType;

#define EXTI          ((ExtiType*)0x40013C00)
#define NVIC          ((NvicType*)0xE000E100)
#define SYSCFG        ((SyscfgType*)0x40013800)

static ExtiCallback ExtiCallbacks[16] = {0};
static uint8 ExtiLineNumberNvicMap[16] = {6, 7, 8, 9, 10, 23, 23, 23, 23, 23, 40, 40, 40, 40, 40, 40};
/*Calculates which of the four External Interrupt Configuration 
Registers (EXTICR1–EXTICR4) to use, as each register handles 4 lines.*/
void Exti_Init(uint8 LineNumber, uint8 PortName, uint8 EdgeType, ExtiCallback Callback) {
    uint8 sysConfigIndex = LineNumber / 4;
    uint8 sysConfigLogicalBitPosition = LineNumber % 4;

    SYSCFG->EXTICR[sysConfigIndex] &= ~(0x0f << (sysConfigLogicalBitPosition * 4));
    SYSCFG->EXTICR[sysConfigIndex] |= (PortName << (sysConfigLogicalBitPosition * 4));

    ExtiCallbacks[LineNumber] = Callback;

    EXTI->RTSR &= ~(0x01 << LineNumber);
    EXTI->FTSR &= ~(0x01 << LineNumber);

    switch (EdgeType) {
        case EXTI_EDGE_RISING:
            EXTI->RTSR |= (0x01 << LineNumber);
            break;
        case EXTI_EDGE_FALLING:
            EXTI->FTSR |= (0x01 << LineNumber);
            break;
        case EXTI_EDGE_BOTH:
            EXTI->RTSR |= (0x01 << LineNumber);
            EXTI->FTSR |= (0x01 << LineNumber);
            break;
        default:
            break;
    }
}

void Exti_Enable(uint8 LineNumber) {
    EXTI->IMR |= (0x01 << LineNumber);
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ISER[irqNumber / 32] |= (0x01 << (irqNumber % 32));
}

void Exti_Disable(uint8 LineNumber) {
    EXTI->IMR &= ~(0x01 << LineNumber);
    uint8 irqNumber = ExtiLineNumberNvicMap[LineNumber];
    NVIC->NVIC_ICER[irqNumber / 32] |= (0x01 << (irqNumber % 32));
}

void EXTI0_IRQHandler(void) {
    if (ExtiCallbacks[0] != 0) {
        ExtiCallbacks[0]();
    }
    EXTI->PR |= (0x01 << 0);
}

void EXTI1_IRQHandler(void) {
    if (ExtiCallbacks[1]) {
        ExtiCallbacks[1]();
    }
    EXTI->PR |= (0x01 << 1);
}

void EXTI2_IRQHandler(void) {
    if (ExtiCallbacks[2]) {
        ExtiCallbacks[2]();
    }
    EXTI->PR |= (0x01 << 2);
}

void EXTI3_IRQHandler(void) {
    if (ExtiCallbacks[3]) {
        ExtiCallbacks[3]();
    }
    EXTI->PR |= (0x01 << 3);
}

void EXTI4_IRQHandler(void) {
    if (ExtiCallbacks[4]) {
        ExtiCallbacks[4]();
    }
    EXTI->PR |= (0x01 << 4);
}

void EXTI9_5_IRQHandler(void) {
    for (uint8 i = 5; i <= 9; i++) {
        if (EXTI->PR & (0x01 << i)) {
            if (ExtiCallbacks[i]) {
                ExtiCallbacks[i]();
            }
            EXTI->PR |= (0x01 << i);
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    for (uint8 i = 10; i <= 15; i++) {
        if (EXTI->PR & (0x01 << i)) {
            if (ExtiCallbacks[i]) {
                ExtiCallbacks[i]();
            }
            EXTI->PR |= (0x01 << i);
        }
    }
}
