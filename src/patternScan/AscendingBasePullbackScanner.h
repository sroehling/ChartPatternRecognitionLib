/*
 * AscendingBasePullbackScanner.h
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#ifndef ASCENDINGBASEPULLBACKSCANNER_H_
#define ASCENDINGBASEPULLBACKSCANNER_H_

#include "PeriodValSegment.h"
#include "PatternMatch.h"
#include "DoubleRange.h"

// An ascending base consists of 3 pull-backs of 10-20%, followed by a break-out
// above the highest one.
class AscendingBasePullbackScanner {
private:
	DoubleRange minMaxDepthPerc_;
public:
	AscendingBasePullbackScanner(const DoubleRange &minMaxDepthPerc);

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~AscendingBasePullbackScanner();
};

#endif /* ASCENDINGBASEPULLBACKSCANNER_H_ */
