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
#include "DoubleRange.h"


class TrendLineScanner: public PatternScanner {
private:
	PatternScannerPtr trendScanner_;

	void initTrendScanner(const DoubleRange &slopeRange,
			const PatternMatchValidatorPtr &matchConstraint, double maxPercDistanceToLineEquation);
public:
	// matchConstraint is a constraint which must match for the whole trend line. The slope of the individual
	// segments must be within the given slope range and also the pattern as a whole must match matchContraint.
	TrendLineScanner(const DoubleRange &slopeRange,const PatternMatchValidatorPtr &matchConstraint);

	TrendLineScanner(const DoubleRange &slopeRange);
	TrendLineScanner(const DoubleRange &slopeRange, double maxPercDistToLineEquation);

	static const DoubleRange DOWNTREND_SLOPE_RANGE;
	static const DoubleRange UPTREND_SLOPE_RANGE;
	static const DoubleRange FLAT_SLOPE_RANGE;

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~TrendLineScanner();
};

#endif /* TRENDLINESCANNER_H_ */
