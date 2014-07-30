/*
 * WedgeScannerEngine.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef WEDGESCANNERENGINE_H_
#define WEDGESCANNERENGINE_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"

// WedgeScannerEngine is a scanner for a category of patterns where 2 trend lines come together in an
// apex. This can include descending triangle (downward upper trend line, flat lower trend line),
// ascending triangle (flat upper trend line, upward sloping lower trend line), or
// triangle (downward sloping upper trend line, upward sloping lower trend line).
class WedgeScannerEngine {
private:
	// Threshold for how far a PeriodVal must be to the intercept between
	// the lower and upper trend line for a break-out to be valid.
	double minPercDistanceToUpperLowerTrendlineIntercept_;

	// The minimum percentage of values in the pattern which must be
	// between the upper and lower trend-line. This allows for some
	// elasticity in the pattern matching, where a few values may
	// whip-saw above and below the trend-lines, but the overall pattern
	// is still considered valid.
	double minPercValsBetweenTrendlines_;

	// scanWedgePatternMatches functionally sub-divides pattern scanning. The main scanPatternMatches
	// scans for the pivot highs and lows, constructs the upper and lower trend-lines, then calls this
	// method with the upper and lower trend-lines.
	PatternMatchListPtr  scanWedgePatternMatches(const PeriodValSegmentPtr &chartVals,
			const LinearEquationPtr &upperTrendLineEq, const LinearEquationPtr &lowerTrendLineEq,
			const PeriodValCltn::iterator &firstPivotHighIter, const PeriodValCltn::iterator &secondPivotHighIter,
			const PeriodValCltn::iterator &firstPivotLowIter, const PeriodValCltn::iterator &secondPivotLowIter) const;

public:
	WedgeScannerEngine();

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
