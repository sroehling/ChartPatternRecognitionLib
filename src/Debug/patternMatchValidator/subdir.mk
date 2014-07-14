################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternMatchValidator/ANDPatternMatchValidator.cpp \
../patternMatchValidator/AnyPatternMatchValidator.cpp \
../patternMatchValidator/CompositePatternMatchValidatorFactory.cpp \
../patternMatchValidator/EndWithinPercentOfStart.cpp \
../patternMatchValidator/HigherLowPatternMatchValidatorFactory.cpp \
../patternMatchValidator/LastValueAbovePointValue.cpp \
../patternMatchValidator/ORPatternMatchValidator.cpp \
../patternMatchValidator/PatternLengthAtLeast.cpp \
../patternMatchValidator/PatternLengthLessThanEqual.cpp \
../patternMatchValidator/PatternMatchValidator.cpp \
../patternMatchValidator/PatternMatchValidatorCreationHelper.cpp \
../patternMatchValidator/PatternMatchValidatorFactory.cpp \
../patternMatchValidator/PatternMatchValueRef.cpp \
../patternMatchValidator/RecoverPercentOfDepth.cpp \
../patternMatchValidator/StaticPatternMatchValidatorFactory.cpp \
../patternMatchValidator/ValueComparisonMatchValidator.cpp 

OBJS += \
./patternMatchValidator/ANDPatternMatchValidator.o \
./patternMatchValidator/AnyPatternMatchValidator.o \
./patternMatchValidator/CompositePatternMatchValidatorFactory.o \
./patternMatchValidator/EndWithinPercentOfStart.o \
./patternMatchValidator/HigherLowPatternMatchValidatorFactory.o \
./patternMatchValidator/LastValueAbovePointValue.o \
./patternMatchValidator/ORPatternMatchValidator.o \
./patternMatchValidator/PatternLengthAtLeast.o \
./patternMatchValidator/PatternLengthLessThanEqual.o \
./patternMatchValidator/PatternMatchValidator.o \
./patternMatchValidator/PatternMatchValidatorCreationHelper.o \
./patternMatchValidator/PatternMatchValidatorFactory.o \
./patternMatchValidator/PatternMatchValueRef.o \
./patternMatchValidator/RecoverPercentOfDepth.o \
./patternMatchValidator/StaticPatternMatchValidatorFactory.o \
./patternMatchValidator/ValueComparisonMatchValidator.o 

CPP_DEPS += \
./patternMatchValidator/ANDPatternMatchValidator.d \
./patternMatchValidator/AnyPatternMatchValidator.d \
./patternMatchValidator/CompositePatternMatchValidatorFactory.d \
./patternMatchValidator/EndWithinPercentOfStart.d \
./patternMatchValidator/HigherLowPatternMatchValidatorFactory.d \
./patternMatchValidator/LastValueAbovePointValue.d \
./patternMatchValidator/ORPatternMatchValidator.d \
./patternMatchValidator/PatternLengthAtLeast.d \
./patternMatchValidator/PatternLengthLessThanEqual.d \
./patternMatchValidator/PatternMatchValidator.d \
./patternMatchValidator/PatternMatchValidatorCreationHelper.d \
./patternMatchValidator/PatternMatchValidatorFactory.d \
./patternMatchValidator/PatternMatchValueRef.d \
./patternMatchValidator/RecoverPercentOfDepth.d \
./patternMatchValidator/StaticPatternMatchValidatorFactory.d \
./patternMatchValidator/ValueComparisonMatchValidator.d 


# Each subdirectory must supply rules for building sources it contributes
patternMatchValidator/%.o: ../patternMatchValidator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchValidator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


