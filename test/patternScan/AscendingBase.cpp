/*
 * AscendingBase.cpp
 *
 *  Created on: Jul 14, 2014
 *      Author: sroehling
 */

#include <boost/test/unit_test.hpp>

#include "PeriodValSegment.h"
#include "AscendingBaseScanner.h"
#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;

BOOST_AUTO_TEST_CASE( AscendingBase_Synthesized )
{
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4, 100.0,  88.0)); // pull-back 1: initial 12% down-trend
	ranges.push_back(TestPerValRange(3,  90.0, 104.0)); // pull-back 1: up-trend going a little higher than initial down-trend
	ranges.push_back(TestPerValRange(3, 103.5,  92.0)); // pull-back 2: next pull-back, with a higher low than pull-back 1
	ranges.push_back(TestPerValRange(4,  92.5, 107.0)); // pull-back 2: up-trend with higher high than pull-back 1
	ranges.push_back(TestPerValRange(4, 106.5,  93.0)); // pull-back 3: up-trend with higher high than pull-back 2
	ranges.push_back(TestPerValRange(4,  93.5,  107.5)); // pull-back 3: up-trend with higher high than pull-back 2
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	AscendingBaseScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	verifyMatchList("AscendingBase_Synthesized",patternMatches,1);
	verifyPatternMatch("AscendingBase_Synthesized match",
			ptime(date(2014,1,1)),ptime(date(2014,1,22)),6,patternMatches->front());

}

BOOST_AUTO_TEST_CASE( AscendingBase_Synthesized_NotEnoughDepth )
{
	BOOST_TEST_MESSAGE("Same as AscendingBase_Synthesized, but first pull-back only 5% (not 10% minimum)");
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4, 100.0,  95.0)); // pull-back 1: Initial down-trend only 5% (must be 10-20%)
	ranges.push_back(TestPerValRange(3,  95.5, 104.0)); // pull-back 1: up-trend going a little higher than initial down-trend
	ranges.push_back(TestPerValRange(3, 103.5,  92.0)); // pull-back 2: next pull-back, with a higher low than pull-back 1
	ranges.push_back(TestPerValRange(4,  92.5, 107.0)); // pull-back 2: up-trend with higher high than pull-back 1
	ranges.push_back(TestPerValRange(4, 106.5,  93.0)); // pull-back 3: up-trend with higher high than pull-back 2
	ranges.push_back(TestPerValRange(4,  93.5,  107.5)); // pull-back 3: up-trend with higher high than pull-back 2
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	AscendingBaseScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	verifyMatchList("AscendingBase_Synthesized",patternMatches,0);

}

BOOST_AUTO_TEST_CASE( AscendingBase_Synthesized_NotHigherLow )
{
	BOOST_TEST_MESSAGE("Same as AscendingBase_Synthesized, but 2nd pull-back has lower-low than 1st (must have higher low)");
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4, 100.0,  88.0)); // pull-back 1: initial 12% down-trend
	ranges.push_back(TestPerValRange(3,  90.0, 104.0)); // pull-back 1: up-trend going a little higher than initial down-trend
	ranges.push_back(TestPerValRange(3, 103.5,  87.0)); // pull-back 2: next pull-back, lower low than first pull-back (match should thus fail)
	ranges.push_back(TestPerValRange(4,  90.5, 107.0)); // pull-back 2: up-trend with higher high than pull-back 1
	ranges.push_back(TestPerValRange(4, 106.5,  93.0)); // pull-back 3: up-trend with higher high than pull-back 2
	ranges.push_back(TestPerValRange(4,  93.5,  107.5)); // pull-back 3: up-trend with higher high than pull-back 2
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	AscendingBaseScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	verifyMatchList("AscendingBase_Synthesized",patternMatches,0);

}

BOOST_AUTO_TEST_CASE( AscendingBase_Synthesized_NotHigherHigh )
{
	BOOST_TEST_MESSAGE("Same as AscendingBase_Synthesized, but 2nd pull-back has lower-low than 1st (must have higher low)");
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4, 100.0,  88.0)); // pull-back 1: initial 12% down-trend
	ranges.push_back(TestPerValRange(3,  90.0, 104.0)); // pull-back 1: up-trend going a little higher than initial down-trend
	ranges.push_back(TestPerValRange(3, 103.5,  92.0)); // pull-back 2: next pull-back, with a higher low than pull-back 1
	ranges.push_back(TestPerValRange(4,  92.5, 103.0)); // pull-back 2: up-trend with without a higher high than pull-back 1 (match should fail)
	ranges.push_back(TestPerValRange(4, 102.5,  93.0)); // pull-back 3: up-trend with higher high than pull-back 2
	ranges.push_back(TestPerValRange(4,  93.5,  107.5)); // pull-back 3: up-trend with higher high than pull-back 2
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	AscendingBaseScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	verifyMatchList("AscendingBase_Synthesized",patternMatches,0);

}

BOOST_AUTO_TEST_CASE( AscendingBase_Synthesized_TooDeep )
{
	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4, 100.0,  75.0)); // pull-back 1: initial 25% down-trend is too deep
	ranges.push_back(TestPerValRange(4,  76.0, 104.0)); // pull-back 1: up-trend going a little higher than initial down-trend
	ranges.push_back(TestPerValRange(3, 103.5,  92.0)); // pull-back 2: next pull-back, with a higher low than pull-back 1
	ranges.push_back(TestPerValRange(4,  92.5, 107.0)); // pull-back 2: up-trend with higher high than pull-back 1
	ranges.push_back(TestPerValRange(4, 106.5,  93.0)); // pull-back 3: up-trend with higher high than pull-back 2
	ranges.push_back(TestPerValRange(4,  93.5,  107.5)); // pull-back 3: up-trend with higher high than pull-back 2
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	AscendingBaseScanner scanner;
	PatternMatchListPtr patternMatches = scanner.scanPatternMatches(chartData);

	verifyMatchList("AscendingBase_Synthesized",patternMatches,0);

}



