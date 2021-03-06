/*
 * TriangleScanner.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef TRIANGLESCANNER_H_
#define TRIANGLESCANNER_H_

#include <WedgeScannerEngine.h>
#include "DoubleRange.h"

class TriangleScanner: public WedgeScannerEngine {
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



    DoubleRange upperTrendLineSlopeRange_;
    DoubleRange lowerTrendLineSlopeRange_;

protected:

    bool pivotsSpacedOut(const ChartSegmentPtr &upperTrendLine,
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
    TriangleScanner(const DoubleRange &upperTrendLineSlopeRange,
                 const DoubleRange &lowerTrendLineSlopeRange);

    virtual ~TriangleScanner();
};

#endif /* TRIANGLESCANNER_H_ */
