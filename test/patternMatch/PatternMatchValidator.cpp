#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"
#include "TimeHelper.h"
#include "TestPerValRange.h"
#include "CompositePatternMatchValidatorFactory.h"
#include "HighestHighLessThanFirstHigh.h"

using namespace testHelper;

BOOST_AUTO_TEST_CASE( PatternMatchValidator_SynthesizePattern )
{
	using namespace boost::gregorian;

	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,105.0,100.0)); // initial down-trend
	ranges.push_back(TestPerValRange(3,101.0,104.5)); // up-trend falling short of down-trend
	ranges.push_back(TestPerValRange(3,104.0,99.0)); // next down-trend, going below the initial down-trend
	ranges.push_back(TestPerValRange(4,99.5,106.0)); // final uptrend, goes above start
	synthesizePeriodValSegment(date(2014,1,1),ranges);
}

BOOST_AUTO_TEST_CASE( CompositeValidatorFactory )
{
    using namespace boost::gregorian;

    TestPerValRangeList ranges;
    ranges.push_back(TestPerValRange(4,100,110.0)); // uptrend
    PeriodValSegmentPtr perValSeg = synthesizePeriodValSegment(date(2014,1,1),ranges);
    PatternMatchPtr patternMatch = testHelper::segmentToPatternMatch(perValSeg);

    CompositePatternMatchValidatorFactory factory;
    factory.addStaticValidator(PatternMatchValidatorPtr(new HighestHighLessThanFirstHigh()));


    PatternMatchValidatorPtr validator = factory.createValidator0();
    bool isValid = validator->validPattern(*patternMatch);
    BOOST_TEST_MESSAGE("CompositeValidatorFactory: valid pattern? " << isValid);
    BOOST_CHECK(isValid == false);
}
