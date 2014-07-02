/*
 * TestHelper.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/test/unit_test.hpp>


#include "TestHelper.h"
#include "PatternMatch.h"
#include "TimeHelper.h"
#include "PeriodVal.h"

using namespace boost::posix_time;
using namespace boost::gregorian;


void TestHelper::genPatternMatchListInfo(const std::string prefix, const PatternMatchList &matchList)
{
	for(PatternMatchList::const_iterator matchIter = matchList.begin();
			matchIter != matchList.end(); matchIter++)
	{
		BOOST_TEST_MESSAGE(prefix << ": pattern match: "
				<< "start = " << TimeHelper::formatDate((*matchIter)->firstValue().periodTime())
				<< ", end = " << TimeHelper::formatDate((*matchIter)->lastValue().periodTime())
				<< ", num segments = " << (*matchIter)->numSegments()
			<< ", last close = " << (*matchIter)->lastValue().close()
		);
	}

}

void TestHelper::genPeriodValSegmentInfo(const std::string prefix, const PeriodValSegment &perSegment)
{
	for(PeriodValCltn::iterator segIter = perSegment.segBegin(); segIter != perSegment.segEnd(); segIter++)
	{
		BOOST_TEST_MESSAGE(prefix << ": period val: "
				<< TimeHelper::formatDate((*segIter).periodTime())
				<< ",c=" << (*segIter).close()
		);
	}
}



PeriodVal TestHelper::testPeriodVal(unsigned int year, unsigned int month, unsigned int day, double val, unsigned int vol)
{
	ptime perTime(date(year,month,day));
	PeriodVal perVal(perTime,val,val,val,val,vol);
	return perVal;
}

