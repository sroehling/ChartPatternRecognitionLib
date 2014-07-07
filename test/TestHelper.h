/*
 * TestUtil.h
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#ifndef TESTHELPER_H_
#define TESTHELPER_H_

#include "PeriodVal.h"
#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class TestHelper {

public:
	static PeriodVal testPeriodVal(unsigned int year, unsigned int month,
					unsigned int day, double val, unsigned int vol);

	static void genPatternMatchInfo(const std::string &prefix, const PatternMatchPtr &patternMatch);
	static void genPatternMatchListInfo(const std::string prefix, const PatternMatchList &matchList);

	static void verifyMatchList(const std::string prefix,
			const PatternMatchListPtr &matchList, unsigned int expectedNumMatches);

	static void genPeriodValSegmentInfo(const std::string prefix, const PeriodValSegment &perSegment);

	static void verifyPatternMatch(const std::string &prefix,const boost::posix_time::ptime &expectedStart,
			const boost::posix_time::ptime &expectedEnd, unsigned int expectedSegments,
			const PatternMatchPtr &patternMatch);

	static boost::posix_time::ptime dateToTime(unsigned int year, unsigned int month, unsigned int day);


};

#endif /* TESTHELPER_H_ */
