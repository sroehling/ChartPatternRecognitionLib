################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternMatch/AnyPatternMatchValidator.cpp \
../patternMatch/EndWithinPercentOfStart.cpp \
../patternMatch/LastValueAbovePointValue.cpp \
../patternMatch/ORPatternMatchValidator.cpp \
../patternMatch/PatternLengthAtLeast.cpp \
../patternMatch/PatternLengthLessThanEqual.cpp \
../patternMatch/PatternMatch.cpp \
../patternMatch/PatternMatchValidator.cpp \
../patternMatch/PatternMatchValueRef.cpp \
../patternMatch/ValueComparisonMatchValidator.cpp 

OBJS += \
./patternMatch/AnyPatternMatchValidator.o \
./patternMatch/EndWithinPercentOfStart.o \
./patternMatch/LastValueAbovePointValue.o \
./patternMatch/ORPatternMatchValidator.o \
./patternMatch/PatternLengthAtLeast.o \
./patternMatch/PatternLengthLessThanEqual.o \
./patternMatch/PatternMatch.o \
./patternMatch/PatternMatchValidator.o \
./patternMatch/PatternMatchValueRef.o \
./patternMatch/ValueComparisonMatchValidator.o 

CPP_DEPS += \
./patternMatch/AnyPatternMatchValidator.d \
./patternMatch/EndWithinPercentOfStart.d \
./patternMatch/LastValueAbovePointValue.d \
./patternMatch/ORPatternMatchValidator.d \
./patternMatch/PatternLengthAtLeast.d \
./patternMatch/PatternLengthLessThanEqual.d \
./patternMatch/PatternMatch.d \
./patternMatch/PatternMatchValidator.d \
./patternMatch/PatternMatchValueRef.d \
./patternMatch/ValueComparisonMatchValidator.d 


# Each subdirectory must supply rules for building sources it contributes
patternMatch/%.o: ../patternMatch/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


