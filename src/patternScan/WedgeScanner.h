/*
 * WedgeScanner.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef WEDGESCANNER_H_
#define WEDGESCANNER_H_

#include <WedgeScannerEngine.h>

class WedgeScanner: public WedgeScannerEngine {
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


	// The number of periods from the first pivot high to the intercept
	// of the upper and lower trend line. Only works when the upper and
	// lower trend line in fact have an intercept.
	double numPeriodsToIntercept(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const;

protected:

    bool pivotsSpacedOut(const ChartSegmentPtr &upperTrendLine,
                           const ChartSegmentPtr &lowerTrendLine) const;
    bool pivotsInterleaved(const ChartSegmentPtr &upperTrendLine,
                           const ChartSegmentPtr &lowerTrendLine) const;
    bool interceptAfter2ndLowerAndUpperPivot(const ChartSegmentPtr &upperTrendLine,
                        const ChartSegmentPtr &lowerTrendLine) const;


	virtual bool validTrendLines(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const;

	// Calculate the minimum and maximum number of periods for a valid
	// pattern match.
	virtual unsigned int maxPatternPeriods(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const;
	virtual unsigned int minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const;

	// The findPatternMatch method is still virtual
public:
	WedgeScanner();

	virtual ~WedgeScanner();
};

#endif /* WEDGESCANNER_H_ */
