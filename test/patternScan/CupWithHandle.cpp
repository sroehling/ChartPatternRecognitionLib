#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodVal.h"
#include "PatternScannerEngine.h"
#include "SegmentConstraint.h"
#include "SegmentValsCloseToLinearEq.h"
#include "SegmentListConstraint.h"
#include "SlopeIncreasesConstraint.h"
#include "PatternMatchValidator.h"
#include "EndWithinPercentOfStart.h"
#include "PeriodValSegment.h"
#include "CupScanner.h"
#include "CupWithHandleScanner.h"
#include "ORPatternMatchValidator.h"
#include "PatternMatchValidator.h"
#include "TestHelper.h"
#include "VScanner.h"
#include "BreakoutAboveFirstHighValidatorFactory.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( MultiSegmentPatternScannerEngine_SAVE_20130722 )
{

	// "typical price" data for SAVE, starting on 2013-07-22,
	// where typical price is (high + low + close)/3.0

	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

	BOOST_TEST_MESSAGE("CupWithHandle_SAVE_20130722");

	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq(3.0));
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart(8.0));

	PatternScannerEngine scanner(segConstraint,segListConstraint,patternMatchValidator);

	genPeriodValSegmentInfo("Cup data",*chartData);

	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

    verifyMatchList("MultiSegmentPatternScannerEngine_SAVE_20130722",patternMatches,2);

	PatternMatchPtr thePatternMatch = patternMatches->front();
	BOOST_CHECK(segListConstraint->validSegments(thePatternMatch->segments()) == true);

	verifyPatternMatch("Cup Match",
            ptime(date(2013,7,22)),ptime(date(2013,10,7)),4,thePatternMatch);

}


BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722_CupWithHandleScanner )
{
	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

    CupWithHandleScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

    // Except for the last candle plunging below 50% of the depth of the cup,  there would be at least one
    // match with a 3 period handle, right after the cup, but CupWithHandleScanner isn't picking this up.
    // This could be because trend line slope is too shallow.
    verifyMatchList("CupWithHandle_SAVE_20130722_CupScanner",patternMatches,0);
}

BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722_CupWithHandle_V_Shaped_Handle )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");
    BOOST_TEST_MESSAGE("Testing V sub-patterns for RHS of cup with handle");
    PeriodValSegmentPair splitData = chartData->split(8);
    PeriodValSegmentPtr rhsSegData = splitData.second;

    genPeriodValSegmentInfo("V RHS segment data",*rhsSegData);

    // The VScanner below uses the same parameters as in CupWithHandleScanner.cpp. However it doesn't have
    // the validator to ensure the handle doesn't go below 50% of the depth of the cup. The last bar in the
    // pattern plunges below 50%, which causes the cup-with-handle scanning to fail. However, the VScanner
    // here should return 1 pattern match, since it doesn't include this extra constraint.
    VScanner scanner(3.0,2,false);
    scanner.overallValidatorFactory().addFactory(PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));

    PatternMatchListPtr patternMatches = scanner.scanPatternMatches(rhsSegData);

    verifyMatchList("CupWithHandle_SAVE_20130722_CupWithHandle_V_Shaped_Handle",patternMatches,1);
}

