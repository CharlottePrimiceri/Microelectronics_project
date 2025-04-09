################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PRIMICERI_Charlotte.axf

# Tool invocations
PRIMICERI_Charlotte.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Charlie XD\Downloads\blinky (1)\peripherals_lib\flash" -L"C:\Users\Charlie XD\Downloads\blinky (1)\utilities_lib\flash" -Xlinker -Map="PRIMICERI_Charlotte.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "blinky_flash.ld" -o "PRIMICERI_Charlotte.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) PRIMICERI_Charlotte.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "PRIMICERI_Charlotte.axf"; # arm-none-eabi-objcopy -v -O binary "PRIMICERI_Charlotte.axf" "PRIMICERI_Charlotte.bin" ; # checksum -p LPC845 -d "PRIMICERI_Charlotte.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
