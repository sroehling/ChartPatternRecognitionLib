################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternScan/AscendingBase.cpp \
../patternScan/CupWithHandle.cpp \
../patternScan/DoubleBottom.cpp \
../patternScan/MultiPatternScan.cpp \
../patternScan/TrendLine.cpp \
../patternScan/VScanner.cpp 

OBJS += \
./patternScan/AscendingBase.o \
./patternScan/CupWithHandle.o \
./patternScan/DoubleBottom.o \
./patternScan/MultiPatternScan.o \
./patternScan/TrendLine.o \
./patternScan/VScanner.o 

CPP_DEPS += \
./patternScan/AscendingBase.d \
./patternScan/CupWithHandle.d \
./patternScan/DoubleBottom.d \
./patternScan/MultiPatternScan.d \
./patternScan/TrendLine.d \
./patternScan/VScanner.d 


# Each subdirectory must supply rules for building sources it contributes
patternScan/%.o: ../patternScan/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchValidator" -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


