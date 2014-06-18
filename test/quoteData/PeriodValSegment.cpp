#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;


#include "PeriodValSegment.h"

BOOST_AUTO_TEST_CASE( PeriodValSegmentTest )
{
	PeriodValCltnPtr testData(new PeriodValCltn());

	testData->push_back(PeriodVal(ptime(date(2013,1,1)),1.0));
	testData->push_back(PeriodVal(ptime(date(2013,2,1)),2.0));
	testData->push_back(PeriodVal(ptime(date(2013,3,1)),3.0));
	testData->push_back(PeriodVal(ptime(date(2013,4,1)),4.0));
	testData->push_back(PeriodVal(ptime(date(2013,5,1)),5.0));

	PeriodValSegmentPtr testSeg(new PeriodValSegment(testData));

    BOOST_CHECK(testSeg->numVals() == 5);
    BOOST_CHECK(testSeg->firstVal().val() == 1.0);
    BOOST_CHECK(testSeg->lastVal().val() == 5.0);

    // Split in the middle of the segment
    PeriodValSegmentPair splitTest1 = testSeg->split(2);
    BOOST_CHECK(splitTest1.first->numVals() == 2);
    BOOST_CHECK(splitTest1.first->lastVal().val() == 2.0);
    BOOST_CHECK(splitTest1.second->numVals() == 3);
    BOOST_CHECK(splitTest1.second->firstVal().val() == 3.0);
    BOOST_CHECK(splitTest1.second->lastVal().val() == 5.0);

    // Split at the end of the segment
    PeriodValSegmentPair splitTest2 = splitTest1.second->split(3);
    BOOST_CHECK(splitTest2.first->numVals() == 3);
    BOOST_CHECK(splitTest2.first->lastVal().val() == 5.0);
    BOOST_CHECK(splitTest2.second->numVals() == 0);

    // Split at the beginning of the segment
    PeriodValSegmentPair splitTest3 = splitTest1.second->split(0);
    BOOST_CHECK(splitTest3.first->numVals() == 0);
    BOOST_CHECK(splitTest3.second->firstVal().val() == 3.0);
    BOOST_CHECK(splitTest3.second->lastVal().val() == 5.0);
    BOOST_CHECK(splitTest3.second->numVals() == 3);

    // Shift the beginning of the segment to the previous value
    PeriodValSegmentPtr splitTest4 = splitTest1.second->moveSegBeginToPrev();
    BOOST_CHECK(splitTest4->numVals() == 4); // add one value because of the shift
    BOOST_CHECK(splitTest4->firstVal().val() == 2.0);
    BOOST_CHECK(splitTest4->lastVal().val() == 5.0);

}
