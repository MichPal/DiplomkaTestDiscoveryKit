################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f30x.S 

OBJS += \
./startup/startup_stm32f30x.o 

S_UPPER_DEPS += \
./startup/startup_stm32f30x.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3DISCOVERY -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F303xC -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/core" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/device" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/StdPeriph_Driver/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/Utilities" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


