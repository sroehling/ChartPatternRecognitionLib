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

BOOST_AUTO_TEST_CASE( CupWithoutHandle_AMBA )
{

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/AMBA_Daily_2014.csv");

    PatternScannerPtr cupWithoutHandleScanner(new CupWithoutHandleScanner());
    MultiPatternScanner multiCupWithoutHandleScanner(cupWithoutHandleScanner);
    PatternMatchListPtr cupWithoutHandleMatches = multiCupWithoutHandleScanner.scanUniquePatternMatches(chartData);

    // This data includes a mal-formed cup without handle, one which is  V-like and
    // should be disqualified using the CupScanners constraints on the ratio for the length of the
    // flat bottom versus the LHS downtrend.
    //
    // In particular: Scanning was identifying the following as a valid cup without handle, when it shouldn't:
    //            CupWithoutHandle_AMBA: pattern match: start = 2014-01-30, end = 2014-02-27,
    //            num segments = 3, depth % = 27.5741, last close = 33.91

    verifyMatchList("CupWithoutHandle_AMBA",cupWithoutHandleMatches,0);


    BOOST_TEST_MESSAGE("CupWithoutHandle_AMBA");
}
