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

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

void genPivotLowInfo(const PatternMatchListPtr &pivotLows)
{
    for(PatternMatchList::iterator pivLowIter = pivotLows->begin();
            pivLowIter != pivotLows->end(); pivLowIter++)
    {
        BOOST_TEST_MESSAGE("PivotLowScanner: pivot low:"
                << " time=" << (*pivLowIter)->lowestLowVal().periodTime()
                << " (psuedo) x val=" << (*pivLowIter)->lowestLowVal().pseudoXVal()
                << ", lowest low=" << (*pivLowIter)->lowestLow());
    }

}

BOOST_AUTO_TEST_CASE( PivotScanner_VZ_SymetricTriangle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    double maxDistanceToTrendLine = 10.0;
    PatternMatchListPtr pivotHighs = PivotHighScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);
    PatternMatchListPtr pivotLows = PivotLowScanner(maxDistanceToTrendLine).scanPatternMatches(chartData);

    // Each of the pivot highs serves as a potential starting point for the pattern match.
    for(PatternMatchList::iterator pivHighIter = pivotHighs->begin();
            pivHighIter != pivotHighs->end(); pivHighIter++)
    {
        BOOST_TEST_MESSAGE("PivotHighScanner: pivot high:"
                << " time=" << (*pivHighIter)->highestHighVal().periodTime()
                << " (psuedo) x val=" << (*pivHighIter)->highestHighVal().pseudoXVal()
                << ", highest high=" << (*pivHighIter)->highestHigh());
    }


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

