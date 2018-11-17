################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Main/Emulator/pdp/branch.c \
../Main/Emulator/pdp/double.c \
../Main/Emulator/pdp/ea.c \
../Main/Emulator/pdp/itab.c \
../Main/Emulator/pdp/service.c \
../Main/Emulator/pdp/single.c \
../Main/Emulator/pdp/timing.c \
../Main/Emulator/pdp/weird.c 

OBJS += \
./Main/Emulator/pdp/branch.o \
./Main/Emulator/pdp/double.o \
./Main/Emulator/pdp/ea.o \
./Main/Emulator/pdp/itab.o \
./Main/Emulator/pdp/service.o \
./Main/Emulator/pdp/single.o \
./Main/Emulator/pdp/timing.o \
./Main/Emulator/pdp/weird.o 

C_DEPS += \
./Main/Emulator/pdp/branch.d \
./Main/Emulator/pdp/double.d \
./Main/Emulator/pdp/ea.d \
./Main/Emulator/pdp/itab.d \
./Main/Emulator/pdp/service.d \
./Main/Emulator/pdp/single.d \
./Main/Emulator/pdp/timing.d \
./Main/Emulator/pdp/weird.d 


# Each subdirectory must supply rules for building sources it contributes
Main/Emulator/pdp/%.o: ../Main/Emulator/pdp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c11 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -O1 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


