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
#include "CupWithHandleScanner.h"

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

    PatternScannerPtr cupWithHandleScanner(new CupWithHandleScanner());
    MultiPatternScanner multiCupWithHandleScanner(cupWithHandleScanner);
    PatternMatchListPtr cupWithHandleMatches = multiCupWithHandleScanner.scanUniquePatternMatches(chartData);


    verifyMatchList("MultiPatternScan_CELG_Daily (vMatches)",vMatches,24);
    verifyMatchList("MultiPatternScan_CELG_Daily (double bottom)",doubleBottoms,0);
    verifyMatchList("MultiPatternScan_CELG_Daily (cups)",cupMatches,7);

    verifyMatchList("MultiPatternScan_CELG_Daily (cup with handles)",cupWithHandleMatches,1);
    verifyPatternMatch("Cup with Handle Match",
            ptime(date(2014,5,14)),ptime(date(2014,6,4)),6,cupWithHandleMatches->front());

}


BOOST_AUTO_TEST_CASE( MultiPatternScan_Save_Weekly )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/SAVE_Weekly_2013.csv");

    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiVScanner(cupScanner);

    PatternMatchListPtr cupMatches = multiVScanner.scanPatternMatches(chartData);

    verifyMatchList("MultiPatternScan_Save_Weekly (cupMatches)",cupMatches,6);
}


BOOST_AUTO_TEST_CASE( MultiPatternScan_GLD_Weekly )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GLD_Weekly_2013_2014.csv");

    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiVScanner(cupScanner);

    PatternMatchListPtr cupMatches = multiVScanner.scanUniquePatternMatches(chartData);

    // TODO - Several of the 5 matches don't look very much like cups. Most of the values
    // are below the flat segment at the bottom. Need to add a constraint to ensure a reasonable
    // ratio of values for the flat area of a cup close above vs below.
    verifyMatchList("MultiPatternScan_GLD_Weekly (cupMatches)",cupMatches,5);
 }


BOOST_AUTO_TEST_CASE( MultiPatternScan_VZ_SymTriangle_Weekly_2013_2014 )
{
    // When scanning for cup based patterns, there's a few patterns in this data where the bottom of the cup
    // goes above the cup scanner's threshold for being below a percentage of the depth of the down-trend's
    // depth. See the CupScanner.cpp where this constraint is defined.

    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/VZ_SymTriangle_Weekly_2013_2014.csv");

    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiVScanner(cupScanner);

    PatternMatchListPtr cupMatches = multiVScanner.scanUniquePatternMatches(chartData);


    verifyMatchList("MultiPatternScan_VZ_SymTriangle_Weekly_2013_2014 (cupMatches)",cupMatches,5);
}


BOOST_AUTO_TEST_CASE( MultiPatternScan_GMCR_Daily )
{
    PeriodValSegmentPtr chartData = PeriodValSegment::readFromFile("./patternScan/GMCR_Daily_2014.csv");

    PatternScannerPtr vScanner(new VScanner());
    MultiPatternScanner multiVScanner(vScanner);
    PatternMatchListPtr vMatches = multiVScanner.scanUniquePatternMatches(chartData);


    PatternScannerPtr cupScanner(new CupScanner());
    MultiPatternScanner multiCupScanner(cupScanner);
    PatternMatchListPtr cupMatches = multiCupScanner.scanUniquePatternMatches(chartData);

    PatternScannerPtr cupWithHandleScanner(new CupWithHandleScanner());
    MultiPatternScanner multiCupWithHandleScanner(cupWithHandleScanner);
    PatternMatchListPtr cupWithHandleMatches = multiCupWithHandleScanner.scanUniquePatternMatches(chartData);

    verifyMatchList("MultiPatternScan_GMCR_Daily (vMatches)",vMatches,5);

    // TODO - There are a couple more cup-like patterns in this chart. So, it may
    // be necessary to further refine the cup pattern matching to catch these.
    verifyMatchList("MultiPatternScan_GMCR_Daily (cups)",cupMatches,5);

    // Without all the constraints in place, this chart data was matching 2 cup with handles
    // which had handles which closed too deep w.r.t. the LHS cup.
    verifyMatchList("MultiPatternScan_GMCR_Daily (cups with handles)",cupWithHandleMatches,0);

}


