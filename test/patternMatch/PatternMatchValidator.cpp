#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"
#include "TimeHelper.h"
#include "TestPerValRange.h"
#include "CompositePatternMatchValidatorFactory.h"
#include "PrevPatternValueRatioValidatorFactory.h"
#include "HighestHighLessThanFirstHigh.h"
#include "PatternMatchValueRef.h"

using namespace testHelper;
using namespace boost::gregorian;


BOOST_AUTO_TEST_CASE( PatternMatchValidator_SynthesizePattern )
{
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,105.0,100.0)); // initial down-trend
	ranges.push_back(TestPerValRange(3,101.0,104.5)); // up-trend falling short of down-trend
	ranges.push_back(TestPerValRange(3,104.0,99.0)); // next down-trend, going below the initial down-trend
	ranges.push_back(TestPerValRange(4,99.5,106.0)); // final uptrend, goes above start
	synthesizePeriodValSegment(date(2014,1,1),ranges);
}

BOOST_AUTO_TEST_CASE( CompositeValidatorFactory )
{
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

BOOST_AUTO_TEST_CASE( PreviousPatternValueRatio )
{
    TestPerValRangeList previousRanges;
    previousRanges.push_back(TestPerValRange(10,100,110.0)); // uptrend
    PatternMatchPtr prevPatternMatch = synthesizePatternMatch(date(2014,1,1),previousRanges);

    DoubleRange validRatios(0.5,2.0);
    PrevPatternValueRatioValidatorFactory ratioValidatorFactory(validRatios,
               PatternMatchValueRefPtr(new NumPeriodsPatternMatchValueRef()));

    PatternMatchValidatorPtr validator = ratioValidatorFactory.createValidator1(prevPatternMatch);

    BOOST_TEST_MESSAGE("PreviousPatternValueRatio: checking pattern which is too long");
    TestPerValRangeList pat1Ranges;
    pat1Ranges.push_back(TestPerValRange(21,100,110.0)); // too long, more than double length of previous
    PatternMatchPtr pat1 = synthesizePatternMatch(date(2014,1,1),pat1Ranges);
    BOOST_CHECK(validator->validPattern(*pat1) == false);

    BOOST_TEST_MESSAGE("PreviousPatternValueRatio: checking pattern which is too short");
    TestPerValRangeList pat2Ranges;
    pat2Ranges.push_back(TestPerValRange(4,100,110.0)); // too short
    PatternMatchPtr pat2 = synthesizePatternMatch(date(2014,1,1),pat2Ranges);
    BOOST_CHECK(validator->validPattern(*pat2) == false);

    BOOST_TEST_MESSAGE("PreviousPatternValueRatio: checking pattern which within the valid ratio");
    TestPerValRangeList pat3Ranges;
    pat3Ranges.push_back(TestPerValRange(6,100,110.0)); // within valid ratio
    PatternMatchPtr pat3 = synthesizePatternMatch(date(2014,1,1),pat3Ranges);
    BOOST_CHECK(validator->validPattern(*pat3) == true);

    BOOST_TEST_MESSAGE("PreviousPatternValueRatio: checking pattern which within the valid ratio");
    TestPerValRangeList pat4Ranges;
    pat4Ranges.push_back(TestPerValRange(19,100,110.0)); // within valid ratio
    PatternMatchPtr pat4 = synthesizePatternMatch(date(2014,1,1),pat4Ranges);
    BOOST_CHECK(validator->validPattern(*pat4) == true);


}
