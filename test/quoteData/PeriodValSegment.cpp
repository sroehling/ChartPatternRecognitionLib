#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodValSegment.h"
#include "PeriodVal.h"
#include "TestHelper.h"

using namespace testHelper;


BOOST_AUTO_TEST_CASE( PeriodValSegmentTest )
{
	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex=0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

    BOOST_CHECK(testSeg->numVals() == 5);
    BOOST_CHECK(testSeg->firstVal().close() == 1.0);
    BOOST_CHECK(testSeg->lastVal().close() == 5.0);

    // Split in the middle of the segment
    PeriodValSegmentPair splitTest1 = testSeg->split(2);
    BOOST_CHECK(splitTest1.first->numVals() == 2);
    BOOST_CHECK(splitTest1.first->lastVal().close() == 2.0);
    BOOST_CHECK(splitTest1.second->numVals() == 3);
    BOOST_CHECK(splitTest1.second->firstVal().close() == 3.0);
    BOOST_CHECK(splitTest1.second->lastVal().close() == 5.0);

    // Split at the end of the segment
    PeriodValSegmentPair splitTest2 = splitTest1.second->split(3);
    BOOST_CHECK(splitTest2.first->numVals() == 3);
    BOOST_CHECK(splitTest2.first->lastVal().close() == 5.0);
    BOOST_CHECK(splitTest2.second->numVals() == 0);

    // Split at the beginning of the segment
    PeriodValSegmentPair splitTest3 = splitTest1.second->split(0);
    BOOST_CHECK(splitTest3.first->numVals() == 0);
    BOOST_CHECK(splitTest3.second->firstVal().close() == 3.0);
    BOOST_CHECK(splitTest3.second->lastVal().close() == 5.0);
    BOOST_CHECK(splitTest3.second->numVals() == 3);

    // Shift the beginning of the segment to the previous value
    PeriodValSegmentPtr splitTest4 = splitTest1.second->moveSegBeginToPrev();
    BOOST_CHECK(splitTest4->numVals() == 4); // add one value because of the shift
    BOOST_CHECK(splitTest4->firstVal().close() == 2.0);
    BOOST_CHECK(splitTest4->lastVal().close() == 5.0);

    PeriodValSegmentPtr trailing = splitTest1.first->trailingVals();
    BOOST_CHECK(trailing->numVals() == 3);
    BOOST_CHECK(trailing->firstVal().close() == 3.0);
    BOOST_CHECK(trailing->lastVal().close() == 5.0);

    PeriodValSegmentPtr trailingWithLast = splitTest1.first->trailingValsWithLastVal();
    BOOST_CHECK(trailingWithLast->numVals() == 4);
    BOOST_CHECK(trailingWithLast->firstVal().close() == 2.0);
    BOOST_CHECK(trailingWithLast->lastVal().close() == 5.0);
}

BOOST_AUTO_TEST_CASE( PeriodValSegmentMinMax )
{
	PeriodValCltnPtr testData(new PeriodValCltn());

	unsigned int periodValIndex = 0;
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5,periodValIndex++));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

    BOOST_TEST_MESSAGE( "Average days per period (test seg): " << testSeg->averageCalendarDaysPerPeriod() );
    BOOST_CHECK_CLOSE(testSeg->averageCalendarDaysPerPeriod(),30.0,0.1);


    BOOST_CHECK(testSeg->lowestLow() == 1.0);
    BOOST_CHECK(testSeg->highestHigh() == 5.0);

	PeriodValCltnPtr cupData = PeriodVal::readFromFile("./patternScan/SAVE_Cup_Weekly_20130722_20131028.csv");
	PeriodValSegmentPtr cupSeg(new PeriodValSegment(cupData));

	BOOST_TEST_MESSAGE( "Lowest low: " << cupSeg->lowestLow() );
	BOOST_TEST_MESSAGE("Highest high: " << cupSeg->highestHigh());
	BOOST_TEST_MESSAGE("Depth (points): " << cupSeg->depthPoints());
	BOOST_TEST_MESSAGE("Depth (percent): " << cupSeg->depthPercent());
	BOOST_CHECK_CLOSE(cupSeg->lowestLow(),29.65,0.0001);
	BOOST_CHECK_CLOSE(cupSeg->highestHigh(),45.18,0.0001);
    BOOST_CHECK_CLOSE( cupSeg->depthPoints(), 15.53, 0.0001 );
    BOOST_CHECK_CLOSE( cupSeg->depthPercent(), 34.3736, 0.0001 );

    BOOST_CHECK_CLOSE( cupSeg->pointsAtPercentOfDepthBelowHigh(25.0),41.297,0.01);

    // In cupData, thera are 15 weekly periods, starting on 7/22/2013 and ending on 10/28/2013.
    // From the start of the first period, to the start of the last period is 98 days.
    // So, each period (on average) spans 98 / (15-1) = 7 days.
    BOOST_TEST_MESSAGE( "Segment span in months (weekly data): " << cupSeg->segmentSpanCalendarMonths() );
    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << cupSeg->averageCalendarDaysPerPeriod() );
    BOOST_TEST_MESSAGE( "Average periods per year (weekly data): " << cupSeg->averageCalendarPeriodsPerYear() );
    BOOST_CHECK_CLOSE(cupSeg->averageCalendarDaysPerPeriod(),7.0,0.01);
    BOOST_CHECK_CLOSE(cupSeg->segmentSpanCalendarMonths(),3.219,0.1);
    BOOST_CHECK_CLOSE(cupSeg->averageCalendarPeriodsPerYear(),52.17,0.1);

    XYCoord startLine(cupSeg->firstVal().pseudoXVal(),100.0);
    XYCoord endLine(cupSeg->lastVal().pseudoXVal(),114.0);
    LinearEquation testSegmentSlope(startLine,endLine);
    BOOST_TEST_MESSAGE( "Per Period % Change across segment " << cupSeg->perPeriodPercChangeAlongLine(testSegmentSlope) );
    BOOST_TEST_MESSAGE( "CAGR of line across segment " << cupSeg->percChangePerYearAlongLine(testSegmentSlope) );
    BOOST_CHECK_CLOSE(cupSeg->percChangePerYearAlongLine(testSegmentSlope),0.629,0.1);

}

BOOST_AUTO_TEST_CASE( PeriodValSegment_CAGR )
{
    PeriodValCltnPtr yearlyData = PeriodVal::readFromFile("./patternScan/AAPL_Daily_2013.csv");
    PeriodValSegmentPtr yearSeg(new PeriodValSegment(yearlyData));

    // In cupData, thera are 15 weekly periods, starting on 7/22/2013 and ending on 10/28/2013.
    // From the start of the first period, to the start of the last period is 98 days.
    // So, each period (on average) spans 98 / (15-1) = 7 days.
    BOOST_TEST_MESSAGE( "Segment span in months (weekly data): " << yearSeg->segmentSpanCalendarMonths() );
    BOOST_TEST_MESSAGE( "Average days per period (weekly data): " << yearSeg->averageCalendarDaysPerPeriod() );
    BOOST_TEST_MESSAGE( "Average periods per year (weekly data): " << yearSeg->averageCalendarPeriodsPerYear() );
    BOOST_CHECK_CLOSE(yearSeg->averageCalendarDaysPerPeriod(),1.44622,0.01);
    BOOST_CHECK_CLOSE(yearSeg->segmentSpanCalendarMonths(),11.92,0.1);
    BOOST_CHECK_CLOSE(yearSeg->averageCalendarPeriodsPerYear(),252.556,0.1);

    XYCoord startLine(yearSeg->firstVal().pseudoXVal(),100.0);
    XYCoord endLine(yearSeg->lastVal().pseudoXVal(),120.0);
    LinearEquation testSegmentSlope(startLine,endLine);
    BOOST_TEST_MESSAGE( "Per Period % Change across segment " << yearSeg->perPeriodPercChangeAlongLine(testSegmentSlope) );
    BOOST_TEST_MESSAGE( "CAGR of line across segment " << yearSeg->percChangePerYearAlongLine(testSegmentSlope) );
    BOOST_CHECK_CLOSE(yearSeg->percChangePerYearAlongLine(testSegmentSlope),0.201357,0.1);
    BOOST_CHECK_CLOSE(yearSeg->perPeriodPercChangeAlongLine(testSegmentSlope),0.000726645,0.1);


    // TODO - The results below almost certainly look correct, but it may make sense to do some further validation.
    // In particular, why is the negative percentage value slighly different than the positive one? However,
    // since these calculations are used for setting constraints on lines, the results look close enough.
    XYCoord startLineNeg(yearSeg->firstVal().pseudoXVal(),100.0);
    XYCoord endLineNeg(yearSeg->lastVal().pseudoXVal(),80.0);
    LinearEquation testSegmentSlopeNeg(startLineNeg,endLineNeg);
    BOOST_TEST_MESSAGE( "Per Period % Change across segment (negative slope) " << yearSeg->perPeriodPercChangeAlongLine(testSegmentSlopeNeg) );
    BOOST_TEST_MESSAGE( "CAGR of line across segment (negative slope) " << yearSeg->percChangePerYearAlongLine(testSegmentSlopeNeg) );
    BOOST_CHECK_CLOSE(yearSeg->percChangePerYearAlongLine(testSegmentSlopeNeg),-0.201105,0.1);
    BOOST_CHECK_CLOSE(yearSeg->perPeriodPercChangeAlongLine(testSegmentSlopeNeg),-0.000888623,0.1);

}

BOOST_AUTO_TEST_CASE( PeriodValSegment_Splice )
{
	PeriodValCltnPtr testData(new PeriodValCltn());
	unsigned int periodValIndex = 0;
										// <<-- split position 0
	testData->push_back(testPeriodVal(2013,1,1,1.0,1,periodValIndex++));
																	// <<-- split position 1
	testData->push_back(testPeriodVal(2013,2,1,2.0,2,periodValIndex++));
	testData->push_back(testPeriodVal(2013,3,1,3.0,3,periodValIndex++));
	testData->push_back(testPeriodVal(2013,4,1,4.0,4,periodValIndex++));
	testData->push_back(testPeriodVal(2013,5,1,5.0,5,periodValIndex++));
																	// <<-- split position 5

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

	PeriodValSegmentPtr splice0 = testSeg->spliceRange(0,0);
    BOOST_CHECK( splice0->numVals() == 0 );


	PeriodValSegmentPtr splice1 = testSeg->spliceRange(0,2);
    BOOST_CHECK( splice1->numVals() == 2 );
    BOOST_CHECK( splice1->firstVal().periodTime() == dateToTime(2013,1,1));
    BOOST_CHECK( splice1->lastVal().periodTime() == dateToTime(2013,2,1));

	PeriodValSegmentPtr splice2 = testSeg->spliceRange(1,2);
    BOOST_CHECK( splice2->numVals() == 1 );
    BOOST_CHECK( splice2->firstVal().periodTime() == dateToTime(2013,2,1));
    BOOST_CHECK( splice2->lastVal().periodTime() == dateToTime(2013,2,1));

	PeriodValSegmentPtr splice3 = testSeg->spliceRange(1,3);
    BOOST_CHECK( splice3->numVals() == 2 );
    BOOST_CHECK( splice3->firstVal().periodTime() == dateToTime(2013,2,1));
    BOOST_CHECK( splice3->lastVal().periodTime() == dateToTime(2013,3,1));

	PeriodValSegmentPtr splice4 = testSeg->spliceRange(3,5);
    BOOST_CHECK( splice4->numVals() == 2 );
    BOOST_CHECK( splice4->firstVal().periodTime() == dateToTime(2013,4,1));
    BOOST_CHECK( splice4->lastVal().periodTime() == dateToTime(2013,5,1));

	PeriodValSegmentPtr splice5 = testSeg->spliceRange(0,5);
    BOOST_CHECK( splice5->numVals() == 5 );
    BOOST_CHECK( splice5->firstVal().periodTime() == dateToTime(2013,1,1));
    BOOST_CHECK( splice5->lastVal().periodTime() == dateToTime(2013,5,1));

}
