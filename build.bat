@echo off
set "ARM_PATH=C:\ST\STM32CubeIDE_2.1.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740\tools\bin"
set "CC=%ARM_PATH%\arm-none-eabi-gcc.exe"
set "OBJCOPY=%ARM_PATH%\arm-none-eabi-objcopy.exe"
set "SIZE=%ARM_PATH%\arm-none-eabi-size.exe"
set "CFLAGS=-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -g -Wall -fdata-sections -ffunction-sections"
set "INCLUDES=-Icommon -Imcal/rcc -Imcal/gpio -Imcal/exti -Imcal/Adc -Imcal/Timer -Imcal/Pwm -Imcal/Nvic -Ihal/seven_seg -Ihal/Keypad -Ihal/led_bar -Ihal/lcd1602 -Iapp"
set "LDFLAGS=-Tlinker.ld -Wl,--gc-sections --specs=nano.specs -Wl,-Map=SecureKeypadProject.map"

if not exist build mkdir build

echo Compiling...
%CC% %CFLAGS% %INCLUDES% ^
 mcal/rcc/Rcc.c mcal/gpio/Gpio.c mcal/exti/Exti.c mcal/Adc/Adc.c mcal/Timer/Timer.c mcal/Pwm/Pwm.c mcal/Nvic/Nvic.c ^
 hal/seven_seg/SevenSeg.c hal/Keypad/Keypad.c hal/led_bar/LedBar.c hal/lcd1602/Lcd1602.c ^
 app/main.c startup.s ^
 %LDFLAGS% -o build/TheAutoCooler.elf

if %errorlevel% equ 0 (
    echo Build Successful!
    %OBJCOPY% -O ihex build/TheAutoCooler.elf build/TheAutoCooler.hex
    %SIZE% build/TheAutoCooler.elf
    echo Hex file generated at: build\TheAutoCooler.hex
) else (
    echo Build Failed!
)
