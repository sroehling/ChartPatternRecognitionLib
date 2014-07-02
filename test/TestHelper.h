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

class TestHelper {

public:
	static PeriodVal testPeriodVal(unsigned int year, unsigned int month,
					unsigned int day, double val, unsigned int vol);
	static void genPatternMatchListInfo(const std::string prefix, const PatternMatchList &matchList);
	static void genPeriodValSegmentInfo(const std::string prefix, const PeriodValSegment &perSegment);

};

#endif /* TESTHELPER_H_ */
