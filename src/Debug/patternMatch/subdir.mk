################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternMatch/AnyPatternMatchValidator.cpp \
../patternMatch/EndWithinPercentOfStart.cpp \
../patternMatch/ORPatternMatchValidator.cpp \
../patternMatch/PatternMatch.cpp \
../patternMatch/PatternMatchValidator.cpp 

OBJS += \
./patternMatch/AnyPatternMatchValidator.o \
./patternMatch/EndWithinPercentOfStart.o \
./patternMatch/ORPatternMatchValidator.o \
./patternMatch/PatternMatch.o \
./patternMatch/PatternMatchValidator.o 

CPP_DEPS += \
./patternMatch/AnyPatternMatchValidator.d \
./patternMatch/EndWithinPercentOfStart.d \
./patternMatch/ORPatternMatchValidator.d \
./patternMatch/PatternMatch.d \
./patternMatch/PatternMatchValidator.d 


# Each subdirectory must supply rules for building sources it contributes
patternMatch/%.o: ../patternMatch/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


