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
#include "DoubleBottomScanner.h"
#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( DoubleBottomScanner_QCOR_20130819 )
{
	// Test a full double-bottom scan for QCOR in 2013. This unit test is built up from
	// VScanner_QCOR_20130819_LHSofDoubleBottom and VScanner_QCOR_20130819_RHSofDoubleBottom, which
	// test the LHS and RHS of the double-bottom individually. The number of pattern matches
	// should therefore be the number of matches from VScanner_QCOR_20130819_LHSofDoubleBottom
	// multiplied times the number of matches from VScanner_QCOR_20130819_RHSofDoubleBottom.

	PeriodValCltnPtr vData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr chartData(new PeriodValSegment(vData));
	TestHelper::genPeriodValSegmentInfo("Double bottom segment data",*chartData);

	DoubleBottomScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("Number of pattern matches: " << patternMatches->size());
	BOOST_REQUIRE(patternMatches->size() == 480);
	TestHelper::genPatternMatchListInfo("Double bottom match",*patternMatches);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 12);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("Double bottom pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,8,26)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2014,2,18)) == lastVal.periodTime());

}
