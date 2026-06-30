/* Simplified Startup File for STM32F4 */
.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler

/* Vector Table */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word	MemManage_Handler
  .word	BusFault_Handler
  .word	UsageFault_Handler
  .word	0
  .word	0
  .word	0
  .word	0
  .word	SVC_Handler
  .word	DebugMon_Handler
  .word	0
  .word	PendSV_Handler
  .word	SysTick_Handler

  /* External Interrupts */
  .word	WWDG_IRQHandler                   /* Window WatchDog              */                                        
  .word	PVD_IRQHandler                    /* PVD through EXTI Line detection */                        
  .word	TAMP_STAMP_IRQHandler             /* Tamper and TimeStamps through the EXTI line */            
  .word	RTC_WKUP_IRQHandler               /* RTC Wakeup through the EXTI line */                      
  .word	FLASH_IRQHandler                  /* FLASH                        */                                          
  .word	RCC_IRQHandler                    /* RCC                          */                                            
  .word	EXTI0_IRQHandler                  /* EXTI Line0                   */                          
  .word	EXTI1_IRQHandler                  /* EXTI Line1                   */                          
  .word	EXTI2_IRQHandler                  /* EXTI Line2                   */                          
  .word	EXTI3_IRQHandler                  /* EXTI Line3                   */                          
  .word	EXTI4_IRQHandler                  /* EXTI Line4                   */                          
  .word	DMA1_Stream0_IRQHandler           /* DMA1 Stream 0                */                  
  .word	DMA1_Stream1_IRQHandler           /* DMA1 Stream 1                */                  
  .word	DMA1_Stream2_IRQHandler           /* DMA1 Stream 2                */                  
  .word	DMA1_Stream3_IRQHandler           /* DMA1 Stream 3                */                  
  .word	DMA1_Stream4_IRQHandler           /* DMA1 Stream 4                */                  
  .word	DMA1_Stream5_IRQHandler           /* DMA1 Stream 5                */                  
  .word	DMA1_Stream6_IRQHandler           /* DMA1 Stream 6                */                  
  .word	ADC_IRQHandler                    /* ADC1, ADC2 and ADC3s         */                  
  .word	0                                 /* Reserved                     */                          
  .word	0                                 /* Reserved                     */                          
  .word	0                                 /* Reserved                     */                          
  .word	0                                 /* Reserved                     */                          
  .word	EXTI9_5_IRQHandler                /* External Line[9:5]s          */                          
  .word	TIM1_BRK_TIM9_IRQHandler          /* TIM1 Break and TIM9          */          
  .word	TIM1_UP_TIM10_IRQHandler          /* TIM1 Update and TIM10        */          
  .word	TIM1_TRG_COM_TIM11_IRQHandler      /* TIM1 Trigger and Commutation and TIM11 */
  .word	TIM1_CC_IRQHandler                /* TIM1 Capture Compare         */                          
  .word	TIM2_IRQHandler                   /* TIM2                         */                   
  .word	TIM3_IRQHandler                   /* TIM3                         */                   
  .word	TIM4_IRQHandler                   /* TIM4                         */                   
  .word	I2C1_EV_IRQHandler                /* I2C1 Event                   */                          
  .word	I2C1_ER_IRQHandler                /* I2C1 Error                   */                          
  .word	I2C2_EV_IRQHandler                /* I2C2 Event                   */                          
  .word	I2C2_ER_IRQHandler                /* I2C2 Error                   */                          
  .word	SPI1_IRQHandler                   /* SPI1                         */                   
  .word	SPI2_IRQHandler                   /* SPI2                         */                   
  .word	USART1_IRQHandler                 /* USART1                       */                   
  .word	USART2_IRQHandler                 /* USART2                       */                   
  .word	0                                 /* Reserved                     */                          
  .word	EXTI15_10_IRQHandler              /* External Line[15:10]s        */

.section .text.Default_Handler,"ax",%progbits
Default_Handler:
  b Default_Handler
  .size Default_Handler, .-Default_Handler

/* Map all handlers to Default_Handler unless defined elsewhere */
.macro IRQ handler
  .weak \handler
  .set \handler, Default_Handler
.endm

IRQ NMI_Handler
IRQ HardFault_Handler
IRQ MemManage_Handler
IRQ BusFault_Handler
IRQ UsageFault_Handler
IRQ SVC_Handler
IRQ DebugMon_Handler
IRQ PendSV_Handler
IRQ SysTick_Handler
IRQ WWDG_IRQHandler
IRQ PVD_IRQHandler
IRQ TAMP_STAMP_IRQHandler
IRQ RTC_WKUP_IRQHandler
IRQ FLASH_IRQHandler
IRQ RCC_IRQHandler
IRQ EXTI0_IRQHandler
IRQ EXTI1_IRQHandler
IRQ EXTI2_IRQHandler
IRQ EXTI3_IRQHandler
IRQ EXTI4_IRQHandler
IRQ DMA1_Stream0_IRQHandler
IRQ DMA1_Stream1_IRQHandler
IRQ DMA1_Stream2_IRQHandler
IRQ DMA1_Stream3_IRQHandler
IRQ DMA1_Stream4_IRQHandler
IRQ DMA1_Stream5_IRQHandler
IRQ DMA1_Stream6_IRQHandler
IRQ ADC_IRQHandler
IRQ EXTI9_5_IRQHandler
IRQ TIM1_BRK_TIM9_IRQHandler
IRQ TIM1_UP_TIM10_IRQHandler
IRQ TIM1_TRG_COM_TIM11_IRQHandler
IRQ TIM1_CC_IRQHandler
IRQ TIM2_IRQHandler
IRQ TIM3_IRQHandler
IRQ TIM4_IRQHandler
IRQ I2C1_EV_IRQHandler
IRQ I2C1_ER_IRQHandler
IRQ I2C2_EV_IRQHandler
IRQ I2C2_ER_IRQHandler
IRQ SPI1_IRQHandler
IRQ SPI2_IRQHandler
IRQ USART1_IRQHandler
IRQ USART2_IRQHandler
IRQ EXTI15_10_IRQHandler

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
  /* Set stack pointer */
  ldr   sp, =_estack

  /* Copy .data from FLASH to RAM */
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_etext
  movs r3, #0
  b .L_loop_data

.L_copy_data:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4

.L_loop_data:
  adds r4, r0, r3
  cmp r4, r1
  bcc .L_copy_data

  /* Zero .bss */
  ldr r0, =_sbss
  ldr r1, =_ebss
  movs r2, #0
  b .L_loop_bss

.L_zero_bss:
  str r2, [r0]
  adds r0, r0, #4

.L_loop_bss:
  cmp r0, r1
  bcc .L_zero_bss

  /* Call main */
  bl main
  b .

.size Reset_Handler, .-Reset_Handler
