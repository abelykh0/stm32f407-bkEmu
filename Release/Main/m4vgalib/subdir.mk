################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Main/m4vgalib/timing.cc \
../Main/m4vgalib/vga.cc 

S_UPPER_SRCS += \
../Main/m4vgalib/copy_words.S 

CC_DEPS += \
./Main/m4vgalib/timing.d \
./Main/m4vgalib/vga.d 

OBJS += \
./Main/m4vgalib/copy_words.o \
./Main/m4vgalib/timing.o \
./Main/m4vgalib/vga.o 

S_UPPER_DEPS += \
./Main/m4vgalib/copy_words.d 


# Each subdirectory must supply rules for building sources it contributes
Main/m4vgalib/%.o: ../Main/m4vgalib/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c11 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -O1 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Main/m4vgalib/%.o: ../Main/m4vgalib/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c++0x '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -O1 -Wall -fmessage-length=0 -ffunction-sections -fdata-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


