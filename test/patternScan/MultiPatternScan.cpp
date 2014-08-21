/*
 * MultiPatternScan.cpp
 *
 *  Created on: Jul 21, 2014
 *      Author: sroehling
 */

#include <boost/test/unit_test.hpp>

#include "TestHelper.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "InvertedVScanner.h"
#include "VScanner.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "DoubleBottomScanner.h"
#include "CupScanner.h"

using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace testHelper;


BOOST_AUTO_TEST_CASE( MultiPatternScan_SynthesizedPivotHighs )
{
	using namespace boost::gregorian;

	TestPerValRangeList ranges;
	ranges.push_back(TestPerValRange(4,92.0,100.0)); // initial 8% up-trend
	ranges.push_back(TestPerValRange(3,99.5,92.0)); // down-trend
	ranges.push_back(TestPerValRange(3,92.5,101.5)); // next up-trend
	ranges.push_back(TestPerValRange(4,101.00,95.0)); // final pivot lower
	PeriodValSegmentPtr chartData = synthesizePeriodValSegment(date(2014,1,1),ranges);

	PatternScannerPtr pivotHighScanner(new InvertedVScanner());
	MultiPatternScanner multiScanner(pivotHighScanner);

	PatternMatchListPtr pivotHighs = multiScanner.scanPatternMatches(chartData);
	genPatternMatchListInfo("Complete list of highest high pattern matches",*pivotHighs);

	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(pivotHighs);
	genPatternMatchListInfo("Sorted/Unique highest high pattern matches",*sortedUniquePivots);

	verifyMatchList("MultiPatternScan_SynthesizedPivotHighs",sortedUniquePivots,2);
	verifyPatternMatch("MultiPatternScan_SynthesizedPivotHighs match",
			ptime(date(2014,1,1)),ptime(date(2014,1,8)),2,sortedUniquePivots,0);
	verifyPatternMatch("MultiPatternScan_SynthesizedPivotHighs match",
			ptime(date(2014,1,7)),ptime(date(2014,1,14)),2,sortedUniquePivots,1);

}

BOOST_AUTO_TEST_CASE( MultiPatternScan_CELG_Daily )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/CELG_20140501_20140814_Daily.csv");

    PatternScannerPtr vScanner(new VScanner());
    MultiPatternScanner multiVScanner(vScanner);

    PatternMatchListPtr vMatches = multiVScanner.scanPatternMatches(chartData);

    PatternScannerPtr doubleBottomScanner(new DoubleBottomScanner(DoubleRange(7.0,40.0)));
    MultiPatternScanner multiDblBottomScanner(doubleBottomScanner);
    PatternMatchListPtr doubleBottoms = multiDblBottomScanner.scanUniquePatternMatches(chartData);

    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiCupScanner(cupScanner);
    PatternMatchListPtr cupMatches = multiCupScanner.scanUniquePatternMatches(chartData);


    verifyMatchList("MultiPatternScan_CELG_Daily (vMatches)",vMatches,217);
    verifyMatchList("MultiPatternScan_CELG_Daily (double bottom)",doubleBottoms,0);
    verifyMatchList("MultiPatternScan_CELG_Daily (cups)",cupMatches,16);
}


BOOST_AUTO_TEST_CASE( MultiPatternScan_Save_Weekly )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Weekly_2013.csv");

    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiVScanner(cupScanner);

    PatternMatchListPtr cupMatches = multiVScanner.scanPatternMatches(chartData);

    verifyMatchList("MultiPatternScan_Save_Weekly (cupMatches)",cupMatches,2);
}


