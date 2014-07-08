#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"
#include "TimeHelper.h"
#include "TestPerValRange.h"

BOOST_AUTO_TEST_CASE( PatternMatchValidator )
{
	using namespace boost::gregorian;

	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,105.0,100.0)); // initial down-trend
	ranges.push_back(TestPerValRange(3,101.0,104.5)); // up-trend falling short of down-trend
	ranges.push_back(TestPerValRange(3,104.0,99.0)); // next down-trend, going below the initial down-trend
	ranges.push_back(TestPerValRange(4,99.5,106.0)); // final uptrend, goes above start
	TestHelper::synthesizePeriodValSegment(date(2014,1,1),ranges);
}
