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
	double pivotLowMaxTrendLineDistancePerc_;

	// Threshold for how far a PeriodVal must be to the intercept between
	// the lower and upper trend line for a break-out to be valid.
	double minPercDistanceToUpperLowerTrendlineIntercept_;

	// The minimum percentage of values in the pattern which must be
	// between the upper and lower trend-line. This allows for some
	// elasticity in the pattern matching, where a few values may
	// whip-saw above and below the trend-lines, but the overall pattern
	// is still considered valid.
	double minPercValsBetweenTrendlines_;

	PatternMatchListPtr scanPivotHighs(const PeriodValSegmentPtr &chartVals) const;
	PatternMatchListPtr scanPivotLows(const PeriodValSegmentPtr &chartVals) const;
public:
	WedgeScannerEngine();

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
