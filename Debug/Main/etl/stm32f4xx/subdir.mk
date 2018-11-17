################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Main/etl/stm32f4xx/gpio.cc \
../Main/etl/stm32f4xx/interrupt_table.cc \
../Main/etl/stm32f4xx/interrupts.cc \
../Main/etl/stm32f4xx/rcc.cc 

CC_DEPS += \
./Main/etl/stm32f4xx/gpio.d \
./Main/etl/stm32f4xx/interrupt_table.d \
./Main/etl/stm32f4xx/interrupts.d \
./Main/etl/stm32f4xx/rcc.d 

OBJS += \
./Main/etl/stm32f4xx/gpio.o \
./Main/etl/stm32f4xx/interrupt_table.o \
./Main/etl/stm32f4xx/interrupts.o \
./Main/etl/stm32f4xx/rcc.o 


# Each subdirectory must supply rules for building sources it contributes
Main/etl/stm32f4xx/%.o: ../Main/etl/stm32f4xx/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c++0x '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -fdata-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


