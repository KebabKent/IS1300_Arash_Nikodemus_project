################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/View/OLED_View.c \
../Core/Src/View/TrafficLight_SPI_Driver.c 

OBJS += \
./Core/Src/View/OLED_View.o \
./Core/Src/View/TrafficLight_SPI_Driver.o 

C_DEPS += \
./Core/Src/View/OLED_View.d \
./Core/Src/View/TrafficLight_SPI_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/View/%.o Core/Src/View/%.su Core/Src/View/%.cyclo: ../Core/Src/View/%.c Core/Src/View/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/OLED_Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-View

clean-Core-2f-Src-2f-View:
	-$(RM) ./Core/Src/View/OLED_View.cyclo ./Core/Src/View/OLED_View.d ./Core/Src/View/OLED_View.o ./Core/Src/View/OLED_View.su ./Core/Src/View/TrafficLight_SPI_Driver.cyclo ./Core/Src/View/TrafficLight_SPI_Driver.d ./Core/Src/View/TrafficLight_SPI_Driver.o ./Core/Src/View/TrafficLight_SPI_Driver.su

.PHONY: clean-Core-2f-Src-2f-View

