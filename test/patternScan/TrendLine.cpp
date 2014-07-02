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
#include "TrendLineScanner.h"
#include "LastValueAbovePointValue.h"
#include "ClosePeriodValueRef.h"
#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( TrendLineScanner_QCOR_20130819_RHS_Uptrend )
{
	PeriodValCltnPtr vData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr chartData(new PeriodValSegment(vData));

	BOOST_TEST_MESSAGE("Testing V sub-patterns for RHS of double bottom of QCOR");
	PeriodValSegmentPair splitQCORdata = chartData->split(19);
	PeriodValSegmentPtr rhsSegData = splitQCORdata.second;

	for (PeriodValCltn::iterator rhsDataIter = rhsSegData->segBegin();
			rhsDataIter != rhsSegData->segEnd(); rhsDataIter++)
	{
		BOOST_TEST_MESSAGE(" RHS uptrend data: period data=" << (*rhsDataIter));
	}

	TrendLineScanner scanner(0.5,100.0,15.0);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(rhsSegData);

	BOOST_TEST_MESSAGE("Number of pattern matches (without constraint): " << patternMatches->size());
	BOOST_REQUIRE(patternMatches->size() == 12);
	TestHelper::genPatternMatchListInfo("Matches without threshold",*patternMatches);

	double thresholdVal = 67.5;
	BOOST_TEST_MESSAGE("Re-running trend-line scan but with a constraint on the last value (close)  >" << thresholdVal);

	PeriodValueRefPtr closeRef(new ClosePeriodValueRef());
	PatternMatchValidatorPtr lastValContraint(new LastValueAbovePointValue(closeRef,thresholdVal));
	TrendLineScanner scannerWithContraint(0.5,10.0,lastValContraint);
	patternMatches = scannerWithContraint.scanPatternMatches(rhsSegData);

	BOOST_TEST_MESSAGE("Number of pattern matches (with last value threshold): " << patternMatches->size());

	TestHelper::genPatternMatchListInfo("Matches above threshold",*patternMatches);
	BOOST_REQUIRE(patternMatches->size() == 6);
	for(PatternMatchList::iterator matchIter = patternMatches->begin();
			matchIter != patternMatches->end(); matchIter++)
	{
		BOOST_CHECK((*matchIter)->lastValue().close() >= thresholdVal);
	}


	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 3);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);
	BOOST_TEST_MESSAGE("Uptrend pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2014,1,6)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2014,2,18)) == lastVal.periodTime());


}


BOOST_AUTO_TEST_CASE( TrendLineScanner_QCOR_20130819_RHS_EndOfUptrend )
{
	// This is basically a test of the scanner engine (PatternScannerEngine). The class put a constraint
	// that every segment must be at least 3 PeriodVal (periods of data) in length. By splitting
	// the chart data at position 23, this leaves 3 values to work with. There was also a defect,
	// whereby PatternScanEngine wasn't including the last PeriodVal in matching; so, this unit
	// test verifies the last value is included as well.

	PeriodValCltnPtr vData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr chartData(new PeriodValSegment(vData));

	BOOST_TEST_MESSAGE("Testing trend-line sub-patterns for RHS of double bottom of QCOR");
	PeriodValSegmentPair splitQCORdata = chartData->split(23);
	PeriodValSegmentPtr rhsSegData = splitQCORdata.second;

	for (PeriodValCltn::iterator rhsDataIter = rhsSegData->segBegin();
			rhsDataIter != rhsSegData->segEnd(); rhsDataIter++)
	{
		BOOST_TEST_MESSAGE(" RHS uptrend data: period data=" << (*rhsDataIter));
	}

	TrendLineScanner scanner(-100.0,100.0,100.0); // leave constraints wide open
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(rhsSegData);

	BOOST_TEST_MESSAGE("Number of pattern matches (without constraint): " << patternMatches->size());
	TestHelper::genPatternMatchListInfo("Matches above threshold",*patternMatches);
	BOOST_CHECK(patternMatches->size() == 1);

}

