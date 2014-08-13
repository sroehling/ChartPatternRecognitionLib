/*
 * PivotHighScanner.h
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#ifndef PIVOTHIGHSCANNER_H_
#define PIVOTHIGHSCANNER_H_

#include "PeriodValSegment.h"
#include "PatternMatch.h"

class PivotHighScanner {
private:
	double pivotHighMaxTrendLineDistancePerc_;
public:
	PivotHighScanner();
    PivotHighScanner(double maxTrendLineDistancePerc);

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~PivotHighScanner();
};

#endif /* PIVOTHIGHSCANNER_H_ */
