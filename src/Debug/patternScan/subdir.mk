################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../patternScan/AscendingBasePullbackScanner.cpp \
../patternScan/AscendingBaseScanner.cpp \
../patternScan/CupScanner.cpp \
../patternScan/DoubleBottomScanner.cpp \
../patternScan/InvertedVScanner.cpp \
../patternScan/MultiPatternScanner.cpp \
../patternScan/PatternScannerEngine.cpp \
../patternScan/PivotHighScanner.cpp \
../patternScan/PivotLowScanner.cpp \
../patternScan/ScannerHelper.cpp \
../patternScan/SymetricWedgeScanner.cpp \
../patternScan/TrendLineScanner.cpp \
../patternScan/VScanner.cpp \
../patternScan/WedgeScanner.cpp \
../patternScan/WedgeScannerEngine.cpp 

OBJS += \
./patternScan/AscendingBasePullbackScanner.o \
./patternScan/AscendingBaseScanner.o \
./patternScan/CupScanner.o \
./patternScan/DoubleBottomScanner.o \
./patternScan/InvertedVScanner.o \
./patternScan/MultiPatternScanner.o \
./patternScan/PatternScannerEngine.o \
./patternScan/PivotHighScanner.o \
./patternScan/PivotLowScanner.o \
./patternScan/ScannerHelper.o \
./patternScan/SymetricWedgeScanner.o \
./patternScan/TrendLineScanner.o \
./patternScan/VScanner.o \
./patternScan/WedgeScanner.o \
./patternScan/WedgeScannerEngine.o 

CPP_DEPS += \
./patternScan/AscendingBasePullbackScanner.d \
./patternScan/AscendingBaseScanner.d \
./patternScan/CupScanner.d \
./patternScan/DoubleBottomScanner.d \
./patternScan/InvertedVScanner.d \
./patternScan/MultiPatternScanner.d \
./patternScan/PatternScannerEngine.d \
./patternScan/PivotHighScanner.d \
./patternScan/PivotLowScanner.d \
./patternScan/ScannerHelper.d \
./patternScan/SymetricWedgeScanner.d \
./patternScan/TrendLineScanner.d \
./patternScan/VScanner.d \
./patternScan/WedgeScanner.d \
./patternScan/WedgeScannerEngine.d 


# Each subdirectory must supply rules for building sources it contributes
patternScan/%.o: ../patternScan/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -I/usr/local/include -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/math" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/quoteData" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegment" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/chartSegmentList" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatch" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternScan" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/date" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchFilter" -I"/Users/sroehling/Development/workspace/PatternRecognitionLib/src/patternMatchValidator" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


