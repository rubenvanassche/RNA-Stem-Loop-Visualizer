################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/UI/main.cpp \
../src/UI/mainwindow.cpp 

CXX_SRCS += \
../src/UI/moc_mainwindow.cxx 

OBJS += \
./src/UI/main.o \
./src/UI/mainwindow.o \
./src/UI/moc_mainwindow.o 

CPP_DEPS += \
./src/UI/main.d \
./src/UI/mainwindow.d 

CXX_DEPS += \
./src/UI/moc_mainwindow.d 


# Each subdirectory must supply rules for building sources it contributes
src/UI/%.o: ../src/UI/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/UI/%.o: ../src/UI/%.cxx
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


