################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CFG.cpp \
../src/CNF.cpp \
../src/LLParser.cpp \
../src/LLParserInputOutput.cpp \
../src/PDA.cpp \
../src/RNAslv.cpp \
../src/Tests.cpp \
../src/Turing.cpp \
../src/runCYK.cpp \
../src/runLLParserInput.cpp \
../src/runPDAInput.cpp \
../src/runTuringInput.cpp \
../src/test_CFG.cpp \
../src/test_CNF.cpp \
../src/test_LLParser.cpp \
../src/test_PDA.cpp \
../src/test_Turing.cpp 

OBJS += \
./src/CFG.o \
./src/CNF.o \
./src/LLParser.o \
./src/LLParserInputOutput.o \
./src/PDA.o \
./src/RNAslv.o \
./src/Tests.o \
./src/Turing.o \
./src/runCYK.o \
./src/runLLParserInput.o \
./src/runPDAInput.o \
./src/runTuringInput.o \
./src/test_CFG.o \
./src/test_CNF.o \
./src/test_LLParser.o \
./src/test_PDA.o \
./src/test_Turing.o 

CPP_DEPS += \
./src/CFG.d \
./src/CNF.d \
./src/LLParser.d \
./src/LLParserInputOutput.d \
./src/PDA.d \
./src/RNAslv.d \
./src/Tests.d \
./src/Turing.d \
./src/runCYK.d \
./src/runLLParserInput.d \
./src/runPDAInput.d \
./src/runTuringInput.d \
./src/test_CFG.d \
./src/test_CNF.d \
./src/test_LLParser.d \
./src/test_PDA.d \
./src/test_Turing.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


