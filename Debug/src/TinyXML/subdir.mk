################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TinyXML/tinystr.cpp \
../src/TinyXML/tinyxml.cpp \
../src/TinyXML/tinyxmlerror.cpp \
../src/TinyXML/tinyxmlparser.cpp 

OBJS += \
./src/TinyXML/tinystr.o \
./src/TinyXML/tinyxml.o \
./src/TinyXML/tinyxmlerror.o \
./src/TinyXML/tinyxmlparser.o 

CPP_DEPS += \
./src/TinyXML/tinystr.d \
./src/TinyXML/tinyxml.d \
./src/TinyXML/tinyxmlerror.d \
./src/TinyXML/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/TinyXML/%.o: ../src/TinyXML/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


