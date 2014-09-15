#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "SymetricWedgeScanner.h"
#include "MultiPatternScanner.h"
#include "PivotHighScanner.h"
#include "PivotLowScanner.h"
#include "CupScanner.h"
#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"
#include "PatternMatchValidator.h"
#include "PivotHighScanner.h"
#include "PivotLowScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;


BOOST_AUTO_TEST_CASE( PivotScanner_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    double maxDistanceToTrendLine = 10.0;
    PatternMatchListPtr pivotHighs = PivotHighScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);
    PatternMatchListPtr pivotLows = PivotLowScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);

    genPivotHighInfo(pivotHighs);
    genPivotLowInfo(pivotLows);

}

BOOST_AUTO_TEST_CASE( PivotScanner_VZ_SymetricTriangle_lastPivotLow )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_Weekly_Nov2013_May2014.csv");

    double maxDistanceToTrendLine = 20.0;
    PatternMatchListPtr pivotLows = PivotLowScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);

    genPivotLowInfo(pivotLows);

/*
    CupScanner scanner;
    PatternMatchListPtr uShapedPivots = scanner.scanPatternMatches(chartData);

    genPivotLowInfo(uShapedPivots);
*/

}


BOOST_AUTO_TEST_CASE( PivotScanner_SynthesizedPivotLows )
{
    // The synthesized period data below is the same as WedgeScannerEngine_SynthesizedPattern
    // test case, but isolates the pivot scanning.

    TestPerValRangeList ranges;
    ranges.push_back(TestPerValRange(4,92.0,100.0)); // up-trend to first pivot high at 100
    ranges.push_back(TestPerValRange(4,98.0,92.0)); // down-trend to first pivot low at 92
    ranges.push_back(TestPerValRange(4,92.5,98.0)); // up-trend to 2nd pivot high at 98 (lower than 1st)
    ranges.push_back(TestPerValRange(4,97.5,94.0)); // down-trend to 2nd pivot low at 94 (higher than 1st)
    ranges.push_back(TestPerValRange(4,94.5,104.0)); // up-trend after 2nd pivot low, including up-side break-out
    PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

    double maxDistanceToTrendLine = 10.0;
    PatternMatchListPtr pivotLows = PivotLowScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);
    genPivotLowInfo(pivotLows);
    BOOST_CHECK_EQUAL(pivotLows->size(),2);

    PatternMatchListPtr pivotHighs = PivotHighScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);
    genPivotHighInfo(pivotHighs);
    BOOST_CHECK_EQUAL(pivotHighs->size(),2);

}


