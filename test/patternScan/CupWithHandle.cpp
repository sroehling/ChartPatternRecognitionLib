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

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722 )
{

	// "typical price" data for SAVE, starting on 2013-07-22,
	// where typical price is (high + low + close)/3.0

	PeriodValCltnPtr cupData = PeriodVal::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

	BOOST_TEST_MESSAGE("CupWithHandle_SAVE_20130722");

	for (PeriodValCltn::iterator cupDataIter = cupData->begin(); cupDataIter != cupData->end(); cupDataIter++)
	{
		BOOST_TEST_MESSAGE(" cup data: period data=" << (*cupDataIter));
	}

	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq(3.0));
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart(8.0));

	PatternScannerEngine scanner(segConstraint,segListConstraint,patternMatchValidator);

	PeriodValSegmentPtr chartData(new PeriodValSegment(cupData));
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_TEST_MESSAGE("CupWithHandle_SAVE_20130722: Number of pattern matches: " << patternMatches->size());
	BOOST_REQUIRE(patternMatches->size() == 2);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 4);

	BOOST_CHECK(segListConstraint->validSegments(thePatternMatch->segments()) == true);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("Cup with Handle: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,7,22)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,7)) == lastVal.periodTime());
}


BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722_CupScanner )
{

	PeriodValCltnPtr cupData = PeriodVal::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");

	CupScanner scanner;

	PeriodValSegmentPtr chartData(new PeriodValSegment(cupData));
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_CHECK(patternMatches->size() == 5);
	BOOST_TEST_MESSAGE("Number of pattern matches: " << patternMatches->size());

	PatternMatchPtr thePatternMatch = patternMatches->front();

	BOOST_TEST_MESSAGE("Number of segments: " << thePatternMatch->numSegments());
	BOOST_CHECK(thePatternMatch->numSegments() == 3);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	BOOST_TEST_MESSAGE("First Period Value: " << firstVal);
	BOOST_TEST_MESSAGE("Last Period Value: " << lastVal);

	BOOST_TEST_MESSAGE("Cup pattern: " << (*thePatternMatch));

	BOOST_CHECK(ptime(date(2013,7,22)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,7)) == lastVal.periodTime());
}
