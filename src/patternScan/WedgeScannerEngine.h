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
#include "ChartSegment.h"

// TODO Refactor to change the name of this class to UpperLowerTrendlinePatternScanner (or similar),
// since derived classes can scan anything from a flat base to a rising/falling/symetric wedge.
// WedgeScannerEngine is a scanner for a category of patterns where 2 trend lines come together in an
// apex. This can include descending triangle (downward upper trend line, flat lower trend line),
// ascending triangle (flat upper trend line, upward sloping lower trend line), or
// triangle (downward sloping upper trend line, upward sloping lower trend line).
class WedgeScannerEngine {

protected:
	// Test if the given upper and lower trend lines are valid with
	// respect to a possible pattern match. For example, this could
	// return true only if the trend lines have an intercept
	// after the first pivot high.
	virtual bool validTrendLines(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const = 0;

	// Calculate the minimum and maximum number of periods for a valid
	// pattern match.
	virtual unsigned int maxPatternPeriods(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const = 0;
	virtual unsigned int minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const = 0;

	// Test if a completed pattern match is found at the location pointed to by
	// currPerValIter.
	virtual PatternMatchPtr findPatternMatch(const PeriodValSegmentPtr &chartVals,
			const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine,
			const PeriodValCltn::iterator &currPerValIter) const = 0;

	// Helper methods
	ChartSegmentPtr createWedgeSegment(const PeriodValSegmentPtr &chartVals,
			const ChartSegmentPtr &upperTrendLine,const PeriodValCltn::iterator &currPerValIter) const;
	bool upperTrendLineBreakout(const PeriodValSegmentPtr &chartVals,
			const ChartSegmentPtr &upperTrendLine,
			const PeriodValCltn::iterator &currPerValIter) const;


public:
	WedgeScannerEngine();

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
