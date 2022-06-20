################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Delta.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Delta.list \

OBJCOPY_BIN += \
Delta.bin \


# All Target
all: Delta.elf secondary-outputs

# Tool invocations
Delta.elf: $(OBJS) $(USER_OBJS) C:\Users\Santiago\Desktop\Santiago\01\ Facultad\Proyecto\ Final\Codigo\ en\ STM\Delta_Firmware\STM32H743ZITX_FLASH.ld
	arm-none-eabi-gcc -o "Delta.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\Santiago\Desktop\Santiago\01 Facultad\Proyecto Final\Codigo en STM\Delta_Firmware\STM32H743ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Delta.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Delta.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Delta.list"
	@echo 'Finished building: $@'
	@echo ' '

Delta.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Delta.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
