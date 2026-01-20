################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/OLED_Drivers/ssd1306.c \
../Drivers/OLED_Drivers/ssd1306_fonts.c 

OBJS += \
./Drivers/OLED_Drivers/ssd1306.o \
./Drivers/OLED_Drivers/ssd1306_fonts.o 

C_DEPS += \
./Drivers/OLED_Drivers/ssd1306.d \
./Drivers/OLED_Drivers/ssd1306_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/OLED_Drivers/%.o Drivers/OLED_Drivers/%.su Drivers/OLED_Drivers/%.cyclo: ../Drivers/OLED_Drivers/%.c Drivers/OLED_Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/OLED_Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-OLED_Drivers

clean-Drivers-2f-OLED_Drivers:
	-$(RM) ./Drivers/OLED_Drivers/ssd1306.cyclo ./Drivers/OLED_Drivers/ssd1306.d ./Drivers/OLED_Drivers/ssd1306.o ./Drivers/OLED_Drivers/ssd1306.su ./Drivers/OLED_Drivers/ssd1306_fonts.cyclo ./Drivers/OLED_Drivers/ssd1306_fonts.d ./Drivers/OLED_Drivers/ssd1306_fonts.o ./Drivers/OLED_Drivers/ssd1306_fonts.su

.PHONY: clean-Drivers-2f-OLED_Drivers

