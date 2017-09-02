################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/syscalls.c \
../src/system_stm32f30x.c 

CPP_SRCS += \
../src/Encoder.cpp \
../src/Hbridge.cpp \
../src/USART.cpp \
../src/main.cpp 

OBJS += \
./src/Encoder.o \
./src/Hbridge.o \
./src/USART.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f30x.o 

C_DEPS += \
./src/syscalls.d \
./src/system_stm32f30x.d 

CPP_DEPS += \
./src/Encoder.d \
./src/Hbridge.d \
./src/USART.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3DISCOVERY -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F303xC -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/core" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/device" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/StdPeriph_Driver/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -DSTM32F30 -DSTM32F3DISCOVERY -DSTM32F3 -DSTM32F303VCTx -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F303xC -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/core" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/CMSIS/device" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/StdPeriph_Driver/inc" -I"C:/Users/Michal/workspace/DiplomkaTestDiscoveryKit/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


