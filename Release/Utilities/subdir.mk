################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/stm32f3_discovery.c \
../Utilities/stm32f3_discovery_l3gd20.c \
../Utilities/stm32f3_discovery_lsm303dlhc.c 

OBJS += \
./Utilities/stm32f3_discovery.o \
./Utilities/stm32f3_discovery_l3gd20.o \
./Utilities/stm32f3_discovery_lsm303dlhc.o 

C_DEPS += \
./Utilities/stm32f3_discovery.d \
./Utilities/stm32f3_discovery_l3gd20.d \
./Utilities/stm32f3_discovery_lsm303dlhc.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/%.o: ../Utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3DISCOVERY -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DUSE_STDPERIPH_DRIVER -DSTM32F303xC -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/core" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/device" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/StdPeriph_Driver/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/Utilities" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


