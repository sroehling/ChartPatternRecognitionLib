################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../chartSegment/ChartSegment.cpp \
../chartSegment/SegmentConstraint.cpp \
../chartSegment/SegmentValsCloseToLinearEq.cpp \
../chartSegment/SlopeWithinRangeConstraint.cpp 

OBJS += \
./chartSegment/ChartSegment.o \
./chartSegment/SegmentConstraint.o \
./chartSegment/SegmentValsCloseToLinearEq.o \
./chartSegment/SlopeWithinRangeConstraint.o 

CPP_DEPS += \
./chartSegment/ChartSegment.d \
./chartSegment/SegmentConstraint.d \
./chartSegment/SegmentValsCloseToLinearEq.d \
./chartSegment/SlopeWithinRangeConstraint.d 


# Each subdirectory must supply rules for building sources it contributes
chartSegment/%.o: ../chartSegment/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


