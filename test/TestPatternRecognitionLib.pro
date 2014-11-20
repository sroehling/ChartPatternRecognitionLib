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
    quoteData/PeriodVal.cpp \
    quoteData/PeriodValSegment.cpp \
    TestHelper.cpp \
    TestPerValRange.cpp \
    patternShape/PatternShape.cpp \
    patternScan/PivotScanner.cpp \
    patternScan/CupWithoutHandle.cpp \
    patternScan/TriangleScanners.cpp \
    patternScan/WedgeScanners.cpp

HEADERS += \
    TestHelper.h \
    TestPerValRange.h


# Configure compiling and linking against libc++ instead of libstdc++
# This is needed to ensure consistency for all components, including
# the boost library.
macx: QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
macx: LIBS += -mmacosx-version-min=10.7 -stdlib=libc++
CONFIG += c++11

## Link with pre-built version of Boost. Using -isystem to include the boost headers supresses
## Warnings from these headers.
DEFINES += BOOST_ALL_DYN_LINK
DEFINES += BOOST_LOG_DYN_LINK
macx: QMAKE_CXXFLAGS += -isystem /usr/local/boost156/include
win32: QMAKE_CXXFLAGS += -isystem c:/boost_1_56_0
macx: LIBS += -L/usr/local/boost156/lib -lboost_date_time-mt -lboost_log-mt -lboost_log_setup-mt -lboost_unit_test_framework-mt
win32:LIBS += -L"C:/boost_1_56_0/stage/lib/" -lboost_date_time-mgw48-mt-1_56 -lboost_log-mgw48-mt-1_56 -lboost_log_setup-mgw48-mt-1_56 -llibboost_unit_test_framework-mgw48-1_56


CONFIG(debug, debug|release) {
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Debug
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Debug\debug
} else {
macx: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_clang_64bit-Release
win32: PATTERNRECOGLIBDIR = build-PatternRecognitionLib-Desktop_Qt_5_3_MinGW_32bit-Release\release
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += NDEBUG
}


# Link with the PatternRecognitionLib and establish a build dependency to re-link if the library changes.
LIBS += -L$$PWD/../$$PATTERNRECOGLIBDIR -lPatternRecognitionLib
PRE_TARGETDEPS += $$PWD/../$$PATTERNRECOGLIBDIR/libPatternRecognitionLib.a

INCLUDEPATH += $$PWD/../src/chartSegment\
    $$PWD/../src/chartSegmentList\
    $$PWD/../src/date\
    $$PWD/../src/math\
    $$PWD/../src/patternMatch\
    $$PWD/../src/patternMatchFilter\
    $$PWD/../src/patternMatchValidator\
    $$PWD/../src/patternScan\
    $$PWD/../src/patternShape\
    $$PWD/../src/quoteData

OTHER_FILES += \
    README.md
