################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Main/sdCard/fatFS/ff.c \
../Main/sdCard/fatFS/ffunicode.c 

OBJS += \
./Main/sdCard/fatFS/ff.o \
./Main/sdCard/fatFS/ffunicode.o 

C_DEPS += \
./Main/sdCard/fatFS/ff.d \
./Main/sdCard/fatFS/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
Main/sdCard/fatFS/%.o: ../Main/sdCard/fatFS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c11 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -O1 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


