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

	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	TestHelper::genPeriodValSegmentInfo("Double bottom segment data",*chartData);

	DoubleBottomScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	TestHelper::verifyMatchList("Double bottom match",patternMatches,1);

	TestHelper::verifyPatternMatch("Double bottom match",
			ptime(date(2013,8,26)),ptime(date(2014,2,18)),5,patternMatches->front());

}

BOOST_AUTO_TEST_CASE( DoubleBottom_Synthesized )
{
	using namespace boost::gregorian;

	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,105.0,100.0)); // initial down-trend
	ranges.push_back(TestPerValRange(3,101.0,104.5)); // up-trend falling short of down-trend
	ranges.push_back(TestPerValRange(3,104.0,99.0)); // next down-trend, going below the initial down-trend
	ranges.push_back(TestPerValRange(4,99.5,106.0)); // final uptrend, goes above start
	PeriodValSegmentPtr chartData = TestHelper::synthesizePeriodValSegment(date(2014,1,1),ranges);

	DoubleBottomScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	TestHelper::verifyMatchList("DoubleBottom_Synthesized",patternMatches,1);
	TestHelper::verifyPatternMatch("DoubleBottom_Synthesized match",
			ptime(date(2014,1,1)),ptime(date(2014,1,14)),4,patternMatches->front());

}

BOOST_AUTO_TEST_CASE( DoubleBottom_Synthesized_RHSHigherLow )
{
	using namespace boost::gregorian;

	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,105.0,100.0)); // initial down-trend
	ranges.push_back(TestPerValRange(3,101.0,104.5)); // up-trend falling short of down-trend
	ranges.push_back(TestPerValRange(3,104.0,101.0)); // next down-trend, *not* going below the initial down-trend
	ranges.push_back(TestPerValRange(4,101.5,106.0)); // final uptrend, goes above start
	PeriodValSegmentPtr chartData = TestHelper::synthesizePeriodValSegment(date(2014,1,1),ranges);

	DoubleBottomScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("Should return 0 matches, since the lowest low on the RHS is not lower than the LHS");
	TestHelper::verifyMatchList("DoubleBottom_Synthesized_RHSHigherLow",patternMatches,0);

}

