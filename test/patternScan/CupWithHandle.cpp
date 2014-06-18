#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodVal.h"
#include "PatternScanner.h"
#include "SegmentConstraint.h"
#include "SegmentValsCloseToLinearEq.h"
#include "SegmentListConstraint.h"
#include "SlopeIncreasesConstraint.h"
#include "PatternMatchValidator.h"
#include "EndWithinPercentOfStart.h"
#include "PeriodValSegment.h"

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722 )
{

	// "typical price" data for SAVE, starting on 2013-07-22,
	// where typical price is (high + low + close)/3.0
	PeriodValCltnPtr saveTypical(new PeriodValCltn());

	saveTypical->push_back(PeriodVal(ptime(date(2013,7,22)),35.37));
	saveTypical->push_back(PeriodVal(ptime(date(2013,7,29)),34.43));
	saveTypical->push_back(PeriodVal(ptime(date(2013,8,5)),34.21));
	saveTypical->push_back(PeriodVal(ptime(date(2013,8,12)),33.31));
	saveTypical->push_back(PeriodVal(ptime(date(2013,8,19)),31.03));
	saveTypical->push_back(PeriodVal(ptime(date(2013,8,26)),31.01));
	saveTypical->push_back(PeriodVal(ptime(date(2013,9,3)),31.59));
	saveTypical->push_back(PeriodVal(ptime(date(2013,9,9)),32.11));
	saveTypical->push_back(PeriodVal(ptime(date(2013,9,16)),34.10));
	saveTypical->push_back(PeriodVal(ptime(date(2013,9,23)),34.35));
	saveTypical->push_back(PeriodVal(ptime(date(2013,9,30)),34.38));
	saveTypical->push_back(PeriodVal(ptime(date(2013,10,7)),35.66));
	saveTypical->push_back(PeriodVal(ptime(date(2013,10,14)),40.68));
	saveTypical->push_back(PeriodVal(ptime(date(2013,10,21)),43.44));
	saveTypical->push_back(PeriodVal(ptime(date(2013,10,28)),42.58));


	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq(2.7));
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart(3.0));

	PatternScanner scanner(segConstraint,segListConstraint,patternMatchValidator);

	PeriodValSegmentPtr chartData(new PeriodValSegment(saveTypical));
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	BOOST_CHECK(patternMatches->size() == 1);
	std::cerr << "Number of pattern matches: " << patternMatches->size() << std::endl;

	PatternMatchPtr thePatternMatch = patternMatches->front();

	std::cerr << "Number of segments: " << thePatternMatch->numSegments() << std::endl;
	BOOST_CHECK(thePatternMatch->numSegments() == 3);

	BOOST_CHECK(segListConstraint->validSegments(thePatternMatch->segments()) == true);

	PeriodVal firstVal = thePatternMatch->firstValue();
	PeriodVal lastVal = thePatternMatch->lastValue();

	std::cerr << "First Period Value: " << firstVal << std::endl;
	std::cerr << "Last Period Value: " << lastVal << std::endl;

	std::cerr << "Cup with Handle: " << (*thePatternMatch) << std::endl;

	BOOST_CHECK(ptime(date(2013,7,22)) == firstVal.periodTime());
	BOOST_CHECK(ptime(date(2013,10,7)) == lastVal.periodTime());
}
