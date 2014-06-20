/*
 * TrendLineScanner.h
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#ifndef TRENDLINESCANNER_H_
#define TRENDLINESCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScannerEngine.h"


class TrendLineScanner: public PatternScanner {
private:
	PatternScannerPtr trendScanner_;
public:
	TrendLineScanner(double minSlope, double maxSlope);

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~TrendLineScanner();
};

#endif /* TRENDLINESCANNER_H_ */
