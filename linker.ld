/* Robust Simple Linker Script for STM32F4 */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 96K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))
    *(.text*)
    *(.rodata*)
    *(.init)
    *(.fini)
    _etext = .;
  } > FLASH

  .data : AT (_etext)
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* Set stack top to end of RAM */
  _estack = ORIGIN(RAM) + LENGTH(RAM);

  /* Discard unused sections */
  /DISCARD/ :
  {
    *(.ARM.attributes)
  }
}
