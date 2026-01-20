################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Model/traffic_state.c 

OBJS += \
./Core/Src/Model/traffic_state.o 

C_DEPS += \
./Core/Src/Model/traffic_state.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Model/%.o Core/Src/Model/%.su Core/Src/Model/%.cyclo: ../Core/Src/Model/%.c Core/Src/Model/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/OLED_Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Model

clean-Core-2f-Src-2f-Model:
	-$(RM) ./Core/Src/Model/traffic_state.cyclo ./Core/Src/Model/traffic_state.d ./Core/Src/Model/traffic_state.o ./Core/Src/Model/traffic_state.su

.PHONY: clean-Core-2f-Src-2f-Model

