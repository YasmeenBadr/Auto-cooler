#include "Nvic.h"

typedef struct
{
    volatile uint32 ISER[8];
    uint32 RESERVED0[24];
    volatile uint32 ICER[8];
} NvicType;

#define NVIC_BASE_ADDR (0xE000E100UL)
#define NVIC ((NvicType *)NVIC_BASE_ADDR)

void Nvic_EnableIrq(uint8 IrqNumber)
{
    NVIC->ISER[IrqNumber / 32U] = (1UL << (IrqNumber % 32U));
}

void Nvic_DisableIrq(uint8 IrqNumber)
{
    NVIC->ICER[IrqNumber / 32U] = (1UL << (IrqNumber % 32U));
}
