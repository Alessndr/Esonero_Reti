################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Constants.c \
../src/Protocol.c \
../src/Socket_Utils.c \
../src/main.c 

C_DEPS += \
./src/Constants.d \
./src/Protocol.d \
./src/Socket_Utils.d \
./src/main.d 

OBJS += \
./src/Constants.o \
./src/Protocol.o \
./src/Socket_Utils.o \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/Constants.d ./src/Constants.o ./src/Protocol.d ./src/Protocol.o ./src/Socket_Utils.d ./src/Socket_Utils.o ./src/main.d ./src/main.o

.PHONY: clean-src

