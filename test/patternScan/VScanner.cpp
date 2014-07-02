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
#include "VScanner.h"
#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( VScanner_QCOR_20130819_LHSofDoubleBottom )
{
	PeriodValCltnPtr allPeriodData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr allPerSegment(new PeriodValSegment(allPeriodData));
	PeriodValSegmentPair splitSeg = allPerSegment->split(11);
	PeriodValSegmentPtr vData = splitSeg.first;
	TestHelper::genPeriodValSegmentInfo("V segment data",*vData);

	double minRHSBelowLHSofVPerc = 40.0;

	VScanner scanner(minRHSBelowLHSofVPerc);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(vData);

	BOOST_TEST_MESSAGE("Number of pattern matches: " << patternMatches->size());

	TestHelper::genPatternMatchListInfo("V Match on LHS",*patternMatches);


	BOOST_REQUIRE(patternMatches->size() == 5);
	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 4);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("V pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,8,26)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,21)) == lastVal.periodTime());

}


BOOST_AUTO_TEST_CASE( VScanner_QCOR_20130819_RHSofDoubleBottom )
{
	PeriodValCltnPtr vData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr chartData(new PeriodValSegment(vData));

	BOOST_TEST_MESSAGE("Testing V sub-patterns for RHS of double bottom of QCOR");
	PeriodValSegmentPair splitQCORdata = chartData->split(8);
	PeriodValSegmentPtr rhsSegData = splitQCORdata.second;
	TestHelper::genPeriodValSegmentInfo("V RHS segment data",*rhsSegData);

	double minPercentRHSBelowLHSofV = 0.0; // RHS must recover entire LHS/downtrend movement

	VScanner scanner(minPercentRHSBelowLHSofV);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(rhsSegData);

	BOOST_TEST_MESSAGE("Number of pattern matches: " << patternMatches->size());
	TestHelper::genPatternMatchListInfo("V Match on RHS",*patternMatches);
	BOOST_REQUIRE(patternMatches->size() == 96);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 8);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("V pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,10,21)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2014,2,18)) == lastVal.periodTime());

}
