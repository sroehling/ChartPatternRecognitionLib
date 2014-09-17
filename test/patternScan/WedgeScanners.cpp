#include <boost/test/unit_test.hpp>
#include "TestHelper.h"
#include "MultiPatternScanner.h"
#include "FallingWedgeScanner.h"
#include "RisingWedgeScanner.h"

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


BOOST_AUTO_TEST_CASE( WedgeScanners_CMG_Rising_Wedge )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CMG_Daily.csv");

    RisingWedgeScanner risingWedgeScanner;
    PatternMatchListPtr risingWedges = risingWedgeScanner.scanPatternMatches(chartData);

    // TODO - 2 of the 3 rising wedge matches look more like ascending triangles. Further
    // adjustment to the acceptable angles for flat versus up/down trend lines need to be made.
    verifyMatchList("WedgeScanners_CMG_Rising_Wedge: rising wedges",risingWedges,3);

}
