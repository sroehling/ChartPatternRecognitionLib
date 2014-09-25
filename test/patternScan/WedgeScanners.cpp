#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "MultiPatternScanner.h"
#include "FallingWedgeScanner.h"
#include "RisingWedgeScanner.h"
#include "RectangleScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( WedgeScanners_GMCR_Falling_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GMCR_Daily_2014.csv");

    FallingWedgeScanner fallingWedgeScanner;
    PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData);

    verifyMatchList("WedgeScanners_GMCR_Falling_Wedge: falling wedges",fallingWedges,2);

}


BOOST_AUTO_TEST_CASE( WedgeScanners_CMG_Falling_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CMG_Daily.csv");

    FallingWedgeScanner fallingWedgeScanner;
    PatternMatchListPtr fallingWedges = fallingWedgeScanner.scanPatternMatches(chartData);

    verifyMatchList("WedgeScanners_CMG_Falling_Wedge: falling wedges",fallingWedges,1);

}

BOOST_AUTO_TEST_CASE( WedgeScanners_VZ_IncompleteRectanglePattern)
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    RectangleScanner rectangleScanner;
    PatternMatchListPtr rectangles = rectangleScanner.scanPatternMatches(chartData);

//    verifyMatchList("WedgeScanners_VZ_IncompletePattern: rectangles",rectangles,1);

    for(int i = 0; i < 100; i++)
    {
        RectangleScanner rectangleScanner;
        PatternMatchListPtr rectangles = rectangleScanner.scanPatternMatches(chartData);
         BOOST_TEST_MESSAGE("WedgeScanners_VZ_IncompletePattern: rectangles " << rectangles->size());

    }
//    verifyMatchList("WedgeScanners_VZ_IncompletePattern: rectangles",rectangles,1);

}



