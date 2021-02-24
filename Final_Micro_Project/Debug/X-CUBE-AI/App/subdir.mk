################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-AI/App/my_model.c \
../X-CUBE-AI/App/my_model_data.c 

OBJS += \
./X-CUBE-AI/App/my_model.o \
./X-CUBE-AI/App/my_model_data.o 

C_DEPS += \
./X-CUBE-AI/App/my_model.d \
./X-CUBE-AI/App/my_model_data.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-AI/App/my_model.o: ../X-CUBE-AI/App/my_model.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-AI/App -I../Middlewares/ST/AI/Inc -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library" -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library/Uart_Ring_Buffer" -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library/ESP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"X-CUBE-AI/App/my_model.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
X-CUBE-AI/App/my_model_data.o: ../X-CUBE-AI/App/my_model_data.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-AI/App -I../Middlewares/ST/AI/Inc -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library" -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library/Uart_Ring_Buffer" -I"C:/Users/alperen/Desktop/MikroUygProje/Stm32_Final_Micro_Project/Final_Micro_Project/My_Library/ESP" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"X-CUBE-AI/App/my_model_data.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

