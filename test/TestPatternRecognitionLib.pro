TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += RunTests.cpp \
    math/LinearEquation.cpp \
    math/XYCoord.cpp \
    patternMatch/PatternMatch.cpp \
    patternMatch/PatternMatchValidator.cpp \
    patternMatchFilter/PatternMatchFilter.cpp \
    patternScan/AscendingBase.cpp \
    patternScan/CupWithHandle.cpp \
    patternScan/DoubleBottom.cpp \
    patternScan/MultiPatternScan.cpp \
    patternScan/TrendLine.cpp \
    patternScan/VScanner.cpp \
    patternScan/WedgeScannerEngine.cpp \
    quoteData/PeriodVal.cpp \
    quoteData/PeriodValSegment.cpp \
    TestHelper.cpp \
    TestPerValRange.cpp \
    patternShape/PatternShape.cpp

HEADERS += \
    TestHelper.h \
    TestPerValRange.h


# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11


## Link with pre-built version of Boost. Using -isystem to include the boost headers supresses
## Warnings from these headers.
DEFINES += BOOST_ALL_DYN_LINK
DEFINES += BOOST_LOG_DYN_LINK
macx: QMAKE_CXXFLAGS += -isystem /usr/local/boost156/include
macx: LIBS += -L/usr/local/boost156/lib -lboost_date_time-mt -lboost_log-mt -lboost_log_setup-mt -lboost_unit_test_framework-mt

macx: LIBS += -L$$PWD/../build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug/ -lPatternRecognitionLib


macx: INCLUDEPATH += $$PWD/../src/chartSegment\
    $$PWD/../src/chartSegmentList\
    $$PWD/../src/date\
    $$PWD/../src/math\
    $$PWD/../src/patternMatch\
    $$PWD/../src/patternMatchFilter\
    $$PWD/../src/patternMatchValidator\
    $$PWD/../src/patternScan\
    $$PWD/../src/patternShape\
    $$PWD/../src/quoteData
