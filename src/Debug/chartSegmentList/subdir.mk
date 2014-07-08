################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../chartSegmentList/AnySegmentListValidConstraint.cpp \
../chartSegmentList/SegmentListConstraint.cpp \
../chartSegmentList/SlopeIncreasesConstraint.cpp 

OBJS += \
./chartSegmentList/AnySegmentListValidConstraint.o \
./chartSegmentList/SegmentListConstraint.o \
./chartSegmentList/SlopeIncreasesConstraint.o 

CPP_DEPS += \
./chartSegmentList/AnySegmentListValidConstraint.d \
./chartSegmentList/SegmentListConstraint.d \
./chartSegmentList/SlopeIncreasesConstraint.d 


# Each subdirectory must supply rules for building sources it contributes
chartSegmentList/%.o: ../chartSegmentList/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchValidator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


