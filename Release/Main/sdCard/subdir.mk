################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Main/sdCard/bsp_driver_sd.c \
../Main/sdCard/diskio.c \
../Main/sdCard/fatfs.c \
../Main/sdCard/ff_gen_drv.c \
../Main/sdCard/sd_diskio.c 

OBJS += \
./Main/sdCard/bsp_driver_sd.o \
./Main/sdCard/diskio.o \
./Main/sdCard/fatfs.o \
./Main/sdCard/ff_gen_drv.o \
./Main/sdCard/sd_diskio.o 

C_DEPS += \
./Main/sdCard/bsp_driver_sd.d \
./Main/sdCard/diskio.d \
./Main/sdCard/fatfs.d \
./Main/sdCard/ff_gen_drv.d \
./Main/sdCard/sd_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
Main/sdCard/%.o: ../Main/sdCard/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=c11 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -D__VFP_FP__ -I"C:/Projects/stm32f407-bkEmu/Main" -I"C:/Projects/stm32f407-bkEmu/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Projects/stm32f407-bkEmu/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Projects/stm32f407-bkEmu/Drivers/CMSIS/Include" -I"C:/Projects/stm32f407-bkEmu/Main/sdCard"  -O1 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


