################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../date/DateValue.cpp 

OBJS += \
./date/DateValue.o 

CPP_DEPS += \
./date/DateValue.d 


# Each subdirectory must supply rules for building sources it contributes
date/%.o: ../date/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


