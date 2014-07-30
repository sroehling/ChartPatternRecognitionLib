/*
 * PivotLowScanner.h
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#ifndef PIVOTLOWSCANNER_H_
#define PIVOTLOWSCANNER_H_

class PivotLowScanner {
private:
	double pivotLowMaxTrendLineDistancePerc_;
public:
	PivotLowScanner();

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~PivotLowScanner();
};

#endif /* PIVOTLOWSCANNER_H_ */
