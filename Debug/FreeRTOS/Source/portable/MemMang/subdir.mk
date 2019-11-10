################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/MemMang/heap_1.c \
../FreeRTOS/Source/portable/MemMang/heap_2.c \
../FreeRTOS/Source/portable/MemMang/heap_3.c \
../FreeRTOS/Source/portable/MemMang/heap_4.c \
../FreeRTOS/Source/portable/MemMang/heap_5.c 

OBJS += \
./FreeRTOS/Source/portable/MemMang/heap_1.o \
./FreeRTOS/Source/portable/MemMang/heap_2.o \
./FreeRTOS/Source/portable/MemMang/heap_3.o \
./FreeRTOS/Source/portable/MemMang/heap_4.o \
./FreeRTOS/Source/portable/MemMang/heap_5.o 

C_DEPS += \
./FreeRTOS/Source/portable/MemMang/heap_1.d \
./FreeRTOS/Source/portable/MemMang/heap_2.d \
./FreeRTOS/Source/portable/MemMang/heap_3.d \
./FreeRTOS/Source/portable/MemMang/heap_4.d \
./FreeRTOS/Source/portable/MemMang/heap_5.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/%.o: ../FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra -fpermissive  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"D:\Development\arm-workspace\2HpMotorController\FreeRTOS\Source\include" -I"D:\Development\arm-workspace\2HpMotorController\FreeRTOS\Source" -I"D:\Development\arm-workspace\2HpMotorController\FreeRTOS\Source\portable\GCC\ARM_CM3" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


