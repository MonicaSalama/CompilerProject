################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CFG.cpp \
../src/DFA.cpp \
../src/DFAMinimizer.cpp \
../src/DFAState.cpp \
../src/Helper.cpp \
../src/LexicalParser.cpp \
../src/NFA.cpp \
../src/NFAState.cpp \
../src/ParsingTable.cpp \
../src/PredictiveParsing.cpp \
../src/TokensGenerator.cpp \
../src/Trie.cpp \
../src/main.cpp 

OBJS += \
./src/CFG.o \
./src/DFA.o \
./src/DFAMinimizer.o \
./src/DFAState.o \
./src/Helper.o \
./src/LexicalParser.o \
./src/NFA.o \
./src/NFAState.o \
./src/ParsingTable.o \
./src/PredictiveParsing.o \
./src/TokensGenerator.o \
./src/Trie.o \
./src/main.o 

CPP_DEPS += \
./src/CFG.d \
./src/DFA.d \
./src/DFAMinimizer.d \
./src/DFAState.d \
./src/Helper.d \
./src/LexicalParser.d \
./src/NFA.d \
./src/NFAState.d \
./src/ParsingTable.d \
./src/PredictiveParsing.d \
./src/TokensGenerator.d \
./src/Trie.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


