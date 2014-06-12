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

using namespace boost::posix_time;
using namespace boost::gregorian;

BOOST_AUTO_TEST_CASE( CupWithHandle_SAVE_20130722 )
{

	PeriodValCltn saveClose; // Closing data for SAVE, starting on 2013-07-22
	saveClose.push_back(PeriodVal(ptime(date(2013,7,22)),35.57));
	saveClose.push_back(PeriodVal(ptime(date(2013,7,29)),34.10));
	saveClose.push_back(PeriodVal(ptime(date(2013,8,5)),34.35));
	saveClose.push_back(PeriodVal(ptime(date(2013,8,12)),31.23));
	saveClose.push_back(PeriodVal(ptime(date(2013,8,19)),31.59));
	saveClose.push_back(PeriodVal(ptime(date(2013,8,26)),31.17));
	saveClose.push_back(PeriodVal(ptime(date(2013,9,3)),31.19));
	saveClose.push_back(PeriodVal(ptime(date(2013,9,9)),33.14));
	saveClose.push_back(PeriodVal(ptime(date(2013,9,16)),34.90));
	saveClose.push_back(PeriodVal(ptime(date(2013,9,23)),34.40));
	saveClose.push_back(PeriodVal(ptime(date(2013,9,30)),34.80));
	saveClose.push_back(PeriodVal(ptime(date(2013,10,7)),39.11));
	saveClose.push_back(PeriodVal(ptime(date(2013,10,14)),43.05));
	saveClose.push_back(PeriodVal(ptime(date(2013,10,21)),41.91));
	saveClose.push_back(PeriodVal(ptime(date(2013,10,28)),42.89));

	SegmentConstraintPtr segConstraint(new SegmentValsCloseToLinearEq());
	SegmentListConstraintPtr segListConstraint(new SlopeIncreasesConstraint());
	PatternMatchValidatorPtr patternMatchValidator(new EndWithinPercentOfStart());

	PatternScanner scanner(segConstraint,segListConstraint,patternMatchValidator);

	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(saveClose);

	std::cout << "Number of pattern matches: " << patternMatches->size() << std::endl;

}
