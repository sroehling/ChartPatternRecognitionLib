/*
 * PivotHighScanner.h
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#ifndef PIVOTHIGHSCANNER_H_
#define PIVOTHIGHSCANNER_H_

class PivotHighScanner {
private:
	double pivotHighMaxTrendLineDistancePerc_;
public:
	PivotHighScanner();

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~PivotHighScanner();
};

#endif /* PIVOTHIGHSCANNER_H_ */
