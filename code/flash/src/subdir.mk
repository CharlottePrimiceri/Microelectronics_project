################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HD44780.c \
../src/MCUXpresso_cr_startup.c \
../src/MCUXpresso_crp.c \
../src/PRIMICERI_Charlotte.c \
../src/system.c \
../src/timer.c 

OBJS += \
./src/HD44780.o \
./src/MCUXpresso_cr_startup.o \
./src/MCUXpresso_crp.o \
./src/PRIMICERI_Charlotte.o \
./src/system.o \
./src/timer.o 

C_DEPS += \
./src/HD44780.d \
./src/MCUXpresso_cr_startup.d \
./src/MCUXpresso_crp.d \
./src/PRIMICERI_Charlotte.d \
./src/system.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS -D__CODE_RED -DCORE_M0PLUS -D__MTB_DISABLE -D__MTB_BUFFER_SIZE=256 -D__LPC84X__ -D__REDLIB__ -I"C:\Users\Charlie XD\Documents\MCUXpressoIDE_11.1.0_3209\workspace\PRIMICERI_Charlotte\inc" -I"C:\Users\Charlie XD\Downloads\blinky (1)\peripherals_lib\inc" -I"C:\Users\Charlie XD\Downloads\blinky (1)\utilities_lib\inc" -I"C:\Users\Charlie XD\Downloads\blinky (1)\common\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


