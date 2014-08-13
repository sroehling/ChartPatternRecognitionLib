/*
 * PivotLowScanner.h
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#ifndef PIVOTLOWSCANNER_H_
#define PIVOTLOWSCANNER_H_

#include "PeriodValSegment.h"
#include "PatternMatch.h"

class PivotLowScanner {
private:
	double pivotLowMaxTrendLineDistancePerc_;
public:
	PivotLowScanner();
    PivotLowScanner(double maxDistanceToTrendLinesPerc);

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~PivotLowScanner();
};

#endif /* PIVOTLOWSCANNER_H_ */
