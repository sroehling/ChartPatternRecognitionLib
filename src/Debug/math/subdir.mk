################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../math/LinearEquation.cpp \
../math/MathHelper.cpp \
../math/XYCoord.cpp 

OBJS += \
./math/LinearEquation.o \
./math/MathHelper.o \
./math/XYCoord.o 

CPP_DEPS += \
./math/LinearEquation.d \
./math/MathHelper.d \
./math/XYCoord.d 


# Each subdirectory must supply rules for building sources it contributes
math/%.o: ../math/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


