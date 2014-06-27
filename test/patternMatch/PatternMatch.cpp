#include <boost/test/unit_test.hpp>

#include "PatternMatch.h"
#include "PeriodVal.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "TestHelper.h"

BOOST_AUTO_TEST_CASE( PatternMatchNumPeriods )
{

	PeriodValCltnPtr seg1Data(new PeriodValCltn());

	seg1Data->push_back(testPeriodVal(2013,1,1,1.0,1));
	seg1Data->push_back(testPeriodVal(2013,2,1,2.0,2));
	seg1Data->push_back(testPeriodVal(2013,3,1,3.0,3));

	PeriodValSegmentPtr seg1(new PeriodValSegment(seg1Data));
	ChartSegmentPtr chartSeg1(new ChartSegment(seg1));

	PeriodValCltnPtr seg2Data(new PeriodValCltn());

	seg2Data->push_back(testPeriodVal(2013,3,1,3.0,3));
	seg2Data->push_back(testPeriodVal(2013,4,1,4.0,4));
	seg2Data->push_back(testPeriodVal(2013,5,1,5.0,5));

	PeriodValSegmentPtr seg2(new PeriodValSegment(seg2Data));
	ChartSegmentPtr chartSeg2(new ChartSegment(seg2));

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
