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

	std::cerr << "CupWithHandle_SAVE_20130722" << std::endl;

	for (PeriodValCltn::iterator cupDataIter = cupData->begin(); cupDataIter != cupData->end(); cupDataIter++)
	{
		std::cerr << " cup data: period data=" << (*cupDataIter) << std::endl;
	}

	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq(3.0));
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart(8.0));

	PatternScannerEngine scanner(segConstraint,segListConstraint,patternMatchValidator);

	PeriodValSegmentPtr chartData(new PeriodValSegment(cupData));
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	std::cerr << "CupWithHandle_SAVE_20130722: Number of pattern matches: " << patternMatches->size() << std::endl;
	BOOST_REQUIRE(patternMatches->size() == 2);

	PatternMatchPtr thePatternMatch = patternMatches->front();

	std::cerr << "Number of segments: " << thePatternMatch->numSegments() << std::endl;
	BOOST_CHECK(thePatternMatch->numSegments() == 4);

	BOOST_CHECK(segListConstraint->validSegments(thePatternMatch->segments()) == true);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	std::cerr << "First Period Value: " << firstVal << std::endl;
	std::cerr << "Last Period Value: " << lastVal << std::endl;

	std::cerr << "Cup with Handle: " << (*thePatternMatch) << std::endl;

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
	std::cerr << "Number of pattern matches: " << patternMatches->size() << std::endl;

	PatternMatchPtr thePatternMatch = patternMatches->front();

	std::cerr << "Number of segments: " << thePatternMatch->numSegments() << std::endl;
	BOOST_CHECK(thePatternMatch->numSegments() == 3);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	std::cerr << "First Period Value: " << firstVal << std::endl;
	std::cerr << "Last Period Value: " << lastVal << std::endl;

	std::cerr << "Cup pattern: " << (*thePatternMatch) << std::endl;

	BOOST_CHECK(ptime(date(2013,7,22)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,7)) == lastVal.periodTime());
}
