/*
 * MultiPatternScanner.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef MULTIPATTERNSCANNER_H_
#define MULTIPATTERNSCANNER_H_

#include "PatternScanner.h"
#include "PeriodValSegment.h"

class MultiPatternScanner {
private:
	PatternScannerPtr singlePatternScanner_;
public:
	MultiPatternScanner(const PatternScannerPtr &singlePatternScanner);

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~MultiPatternScanner();
};

#endif /* MULTIPATTERNSCANNER_H_ */
