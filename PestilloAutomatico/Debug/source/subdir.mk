################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/SysTick.c \
../source/drv_FRDM.c \
../source/drv_display.c \
../source/drv_leds.c \
../source/drv_switch.c \
../source/fsm.c \
../source/gpio.c \
../source/lectora.c \
../source/timer.c 

OBJS += \
./source/App.o \
./source/SysTick.o \
./source/drv_FRDM.o \
./source/drv_display.o \
./source/drv_leds.o \
./source/drv_switch.o \
./source/fsm.o \
./source/gpio.o \
./source/lectora.o \
./source/timer.o 

C_DEPS += \
./source/App.d \
./source/SysTick.d \
./source/drv_FRDM.d \
./source/drv_display.d \
./source/drv_leds.d \
./source/drv_switch.d \
./source/fsm.d \
./source/gpio.d \
./source/lectora.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=0 -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


