#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"

using namespace testHelper;

BOOST_AUTO_TEST_CASE( PatternMatchNumPeriods )
{

	PeriodValCltnPtr testData(new PeriodValCltn());

	testData->push_back(testPeriodVal(2013,1,1,1.0,1));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));
    PeriodValSegmentPair splitTestSeg = testSeg->split(3);


	ChartSegmentPtr chartSeg1(new ChartSegment(splitTestSeg.first));
	ChartSegmentPtr chartSeg2(new ChartSegment(splitTestSeg.second->moveSegBeginToPrev()));

	ChartSegmentList matchSegments;
	matchSegments.push_back(chartSeg1);
	matchSegments.push_back(chartSeg2);

	PatternMatchPtr patternMatch(new PatternMatch(matchSegments));

	BOOST_CHECK(patternMatch->numPeriods()==5);

	ChartSegmentList matchSegments2;
	matchSegments2.push_back(chartSeg1);
	PatternMatchPtr patternMatchSingleSeg(new PatternMatch(matchSegments2));
	BOOST_CHECK(patternMatchSingleSeg->numPeriods()==3);


}
