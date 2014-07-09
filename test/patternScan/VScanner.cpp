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
using namespace testHelper;

BOOST_AUTO_TEST_CASE( VScanner_QCOR_20130819_LHSofDoubleBottom )
{
	PeriodValSegmentPtr allPerSegment = PeriodValSegment::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");

	PeriodValSegmentPair splitSeg = allPerSegment->split(11);
	PeriodValSegmentPtr vData = splitSeg.first;
	genPeriodValSegmentInfo("V segment data",*vData);

	double minRHSBelowLHSofVPerc = 40.0;

	VScanner scanner(minRHSBelowLHSofVPerc);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(vData);

	verifyMatchList("VScanner_QCOR_20130819_LHSofDoubleBottom",patternMatches,1);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	verifyPatternMatch("V Match on LHS",
			ptime(date(2013,8,26)),ptime(date(2013,10,21)),2,thePatternMatch);

}


BOOST_AUTO_TEST_CASE( VScanner_QCOR_20130819_RHSofDoubleBottom )
{
	PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");

	BOOST_TEST_MESSAGE("Testing V sub-patterns for RHS of double bottom of QCOR");
	PeriodValSegmentPair splitQCORdata = chartData->split(8);
	PeriodValSegmentPtr rhsSegData = splitQCORdata.second;

	genPeriodValSegmentInfo("V RHS segment data",*rhsSegData);

	double minPercentRHSBelowLHSofV = 0.0; // RHS must recover entire LHS/downtrend movement

	VScanner scanner(minPercentRHSBelowLHSofV);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(rhsSegData);

	verifyMatchList("VScanner_QCOR_20130819_RHSofDoubleBottom",patternMatches,1);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	verifyPatternMatch("V Match on RHS",
			ptime(date(2013,10,21)),ptime(date(2014,2,18)),3,thePatternMatch);

}
