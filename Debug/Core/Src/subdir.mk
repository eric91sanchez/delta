################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/GlobalFunc.c \
../Core/Src/debug.c \
../Core/Src/gpio.c \
../Core/Src/homing.c \
../Core/Src/interpretaComando.c \
../Core/Src/inverseJacobian.c \
../Core/Src/kinematic.c \
../Core/Src/main.c \
../Core/Src/motor.c \
../Core/Src/statesMachine.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c \
../Core/Src/tim.c \
../Core/Src/trajectory.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/GlobalFunc.o \
./Core/Src/debug.o \
./Core/Src/gpio.o \
./Core/Src/homing.o \
./Core/Src/interpretaComando.o \
./Core/Src/inverseJacobian.o \
./Core/Src/kinematic.o \
./Core/Src/main.o \
./Core/Src/motor.o \
./Core/Src/statesMachine.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o \
./Core/Src/tim.o \
./Core/Src/trajectory.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/GlobalFunc.d \
./Core/Src/debug.d \
./Core/Src/gpio.d \
./Core/Src/homing.d \
./Core/Src/interpretaComando.d \
./Core/Src/inverseJacobian.d \
./Core/Src/kinematic.d \
./Core/Src/main.d \
./Core/Src/motor.d \
./Core/Src/statesMachine.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d \
./Core/Src/tim.d \
./Core/Src/trajectory.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H743xx -DDEBUG -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/STM32H7xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/GlobalFunc.cyclo ./Core/Src/GlobalFunc.d ./Core/Src/GlobalFunc.o ./Core/Src/GlobalFunc.su ./Core/Src/debug.cyclo ./Core/Src/debug.d ./Core/Src/debug.o ./Core/Src/debug.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/homing.cyclo ./Core/Src/homing.d ./Core/Src/homing.o ./Core/Src/homing.su ./Core/Src/interpretaComando.cyclo ./Core/Src/interpretaComando.d ./Core/Src/interpretaComando.o ./Core/Src/interpretaComando.su ./Core/Src/inverseJacobian.cyclo ./Core/Src/inverseJacobian.d ./Core/Src/inverseJacobian.o ./Core/Src/inverseJacobian.su ./Core/Src/kinematic.cyclo ./Core/Src/kinematic.d ./Core/Src/kinematic.o ./Core/Src/kinematic.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor.cyclo ./Core/Src/motor.d ./Core/Src/motor.o ./Core/Src/motor.su ./Core/Src/statesMachine.cyclo ./Core/Src/statesMachine.d ./Core/Src/statesMachine.o ./Core/Src/statesMachine.su ./Core/Src/stm32h7xx_hal_msp.cyclo ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.cyclo ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32h7xx.cyclo ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o ./Core/Src/system_stm32h7xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/trajectory.cyclo ./Core/Src/trajectory.d ./Core/Src/trajectory.o ./Core/Src/trajectory.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

