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

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( VScanner_QCOR_20130819 )
{
	PeriodValCltnPtr vData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	PeriodValSegmentPtr chartData(new PeriodValSegment(vData));

	for (PeriodValCltn::iterator vDataIter = vData->begin(); vDataIter != vData->end(); vDataIter++)
	{
		BOOST_TEST_MESSAGE(" v data: period data=" << (*vDataIter));
	}

	double minPercentOfDowntrend = 80.0;

	VScanner scanner(minPercentOfDowntrend);
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("Number of pattern matches: " << patternMatches->size());
	BOOST_REQUIRE(patternMatches->size() == 2);
	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 3);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("V pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,8,26)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,21)) == lastVal.periodTime());

}
