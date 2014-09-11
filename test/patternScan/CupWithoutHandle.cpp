#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "TestHelper.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "InvertedVScanner.h"
#include "VScanner.h"
#include "DoubleBottomScanner.h"
#include "CupScanner.h"
#include "CupWithoutHandleScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( CupWithoutHandle_GBX )
{

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GBX_Daily_2014.csv");

    PatternScannerPtr cupWithoutHandleScanner(new CupWithoutHandleScanner());
    MultiPatternScanner multiCupWithoutHandleScanner(cupWithoutHandleScanner);
    PatternMatchListPtr cupWithoutHandleMatches = multiCupWithoutHandleScanner.scanUniquePatternMatches(chartData);

    // This data includes a well-formed cup without handle, but also one which is more V-like and
    // should be disqualified using the CupScanners constraints on the ratio for the length of the
    // flat bottom versus the LHS downtrend.

    verifyMatchList("CupWithoutHandle_GBX",cupWithoutHandleMatches,1);

    verifyPatternMatch("CupWithoutHandle_GBX match",
            ptime(date(2014,4,2)),ptime(date(2014,4,22)),3,cupWithoutHandleMatches,0);

    BOOST_TEST_MESSAGE("CupWithoutHandle_GBX");
}
