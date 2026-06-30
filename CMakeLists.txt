cmake_minimum_required(VERSION 3.20)

# Toolchain Path (Update if different)
set(ARM_TOOLCHAIN "C:/ST/STM32CubeIDE_2.1.1/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740/tools/bin")
# Cross-compiling flags
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER "${ARM_TOOLCHAIN}/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER "${ARM_TOOLCHAIN}/arm-none-eabi-gcc.exe")
set(CMAKE_OBJCOPY "${ARM_TOOLCHAIN}/arm-none-eabi-objcopy.exe")
set(CMAKE_SIZE "${ARM_TOOLCHAIN}/arm-none-eabi-size.exe")

project(TheAutoCooler C ASM)

# Compile Options
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -O0 -g -Wall -fdata-sections -ffunction-sections")
set(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -Wl,--gc-sections --specs=nano.specs -Wl,-Map=${PROJECT_NAME}.map")

# Include Directories
include_directories(
    common
    mcal/rcc mcal/gpio mcal/exti mcal/Adc mcal/Timer mcal/Pwm mcal/Nvic
    hal/seven_seg hal/keypad hal/led_bar hal/lcd1602
    app
)

# Source Files
file(GLOB_RECURSE SOURCES 
    "mcal/*.c"
    "hal/*.c"
    "app/*.c"
)

add_executable(${PROJECT_NAME}.elf ${SOURCES} startup.s)

# Post-Build: HEX Generation
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
)
