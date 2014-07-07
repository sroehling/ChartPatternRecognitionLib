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

void TestHelper::genPatternMatchInfo(const std::string &prefix, const PatternMatchPtr &patternMatch)
{
	BOOST_TEST_MESSAGE(prefix << ": pattern match: "
			<< "start = " << TimeHelper::formatDate(patternMatch->firstValue().periodTime())
			<< ", end = " << TimeHelper::formatDate(patternMatch->lastValue().periodTime())
			<< ", num segments = " << patternMatch->numSegments()
		<< ", last close = " << patternMatch->lastValue().close());

}

void TestHelper::genPatternMatchListInfo(const std::string prefix, const PatternMatchList &matchList)
{
	BOOST_TEST_MESSAGE(prefix << ": number of pattern matches = " << matchList.size());

	for(PatternMatchList::const_iterator matchIter = matchList.begin();
			matchIter != matchList.end(); matchIter++)
	{
		TestHelper::genPatternMatchInfo(prefix,*matchIter);
	}

}

void TestHelper::verifyMatchList(const std::string prefix,
		const PatternMatchListPtr &matchList, unsigned int expectedNumMatches)
{
	TestHelper::genPatternMatchListInfo(prefix,*matchList);
	BOOST_REQUIRE(matchList->size() == expectedNumMatches);
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

void TestHelper::verifyPatternMatch(const std::string &prefix,
		const boost::posix_time::ptime &expectedStart,
		const boost::posix_time::ptime &expectedEnd, unsigned int expectedSegments,
		const PatternMatchPtr &patternMatch)
{

	BOOST_TEST_MESSAGE(prefix << ": number of segments= " << patternMatch->numSegments());
	BOOST_CHECK(patternMatch->numSegments() == expectedSegments);

	PeriodVal firstVal = patternMatch->firstValue();
	PeriodVal lastVal = patternMatch->lastValue();

	BOOST_TEST_MESSAGE(prefix << ": first period value: " << firstVal);
	BOOST_TEST_MESSAGE(prefix << ": last period value: " << lastVal);
	TestHelper::genPatternMatchInfo(prefix,patternMatch);

	BOOST_CHECK(expectedStart == firstVal.periodTime());
	BOOST_CHECK(expectedEnd == lastVal.periodTime());

}

boost::posix_time::ptime TestHelper::dateToTime(unsigned int year, unsigned int month, unsigned int day)
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	return ptime(date(year,month,day));
}


PeriodVal TestHelper::testPeriodVal(unsigned int year, unsigned int month, unsigned int day, double val, unsigned int vol)
{
	ptime perTime(date(year,month,day));
	PeriodVal perVal(perTime,val,val,val,val,vol);
	return perVal;
}

