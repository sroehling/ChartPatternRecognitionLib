/*
 * PatternScanner.h
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#ifndef PATTERNSCANNER_H_
#define PATTERNSCANNER_H_

#include <boost/shared_ptr.hpp>
#include "PeriodValSegment.h"
#include "PatternMatch.h"

class PatternScanner {
public:
	PatternScanner() {}

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const = 0;

	virtual ~PatternScanner() {}
};

typedef boost::shared_ptr<PatternScanner> PatternScannerPtr;


#endif /* PATTERNSCANNER_H_ */
