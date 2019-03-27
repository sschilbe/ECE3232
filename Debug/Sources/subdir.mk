################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/acceleration_control.c \
../Sources/communication_controller.c \
../Sources/dac_control.c \
../Sources/ftm_control.c \
../Sources/gpio_control.c \
../Sources/main.c \
../Sources/uart_control.c 

OBJS += \
./Sources/acceleration_control.o \
./Sources/communication_controller.o \
./Sources/dac_control.o \
./Sources/ftm_control.o \
./Sources/gpio_control.o \
./Sources/main.o \
./Sources/uart_control.o 

C_DEPS += \
./Sources/acceleration_control.d \
./Sources/communication_controller.d \
./Sources/dac_control.d \
./Sources/ftm_control.d \
./Sources/gpio_control.d \
./Sources/main.d \
./Sources/uart_control.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MK64FN1M0VMD12" -D"STD_FP_IO=1" -I"../Sources" -I"../Project_Settings/Startup_Code" -I"../SDK/platform/CMSIS/Include" -I"../SDK/platform/devices" -I"../SDK/platform/devices/MK64F12/include" -std=c99 -u _printf_float -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


