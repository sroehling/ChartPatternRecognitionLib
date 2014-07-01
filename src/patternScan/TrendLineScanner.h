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
#include "PatternMatchValidator.h"


class TrendLineScanner: public PatternScanner {
private:
	PatternScannerPtr trendScanner_;

	void initTrendScanner(double minSlope, double maxSlope, const PatternMatchValidatorPtr &matchConstraint);
public:
	// matchConstraint is a constraint which must match for the whole trend line. The slope of the individual
	// segments must be within the given slope range and also the pattern as a whole must match matchContraint.
	TrendLineScanner(double minSlope, double maxSlope,const PatternMatchValidatorPtr &matchConstraint);

	TrendLineScanner(double minSlope, double maxSlope);


	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~TrendLineScanner();
};

#endif /* TRENDLINESCANNER_H_ */
