################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dbox_dumper.cpp \
../src/XGetopt.cpp \
../src/d-box_dumper.cpp \
../src/direntwin.cpp \
../src/spear_parser.cpp 

OBJS += \
./src/Dbox_dumper.o \
./src/XGetopt.o \
./src/d-box_dumper.o \
./src/direntwin.o \
./src/spear_parser.o 

CPP_DEPS += \
./src/Dbox_dumper.d \
./src/XGetopt.d \
./src/d-box_dumper.d \
./src/direntwin.d \
./src/spear_parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<" -mmacosx-version-min=10.6
	@echo 'Finished building: $<'
	@echo ' '


