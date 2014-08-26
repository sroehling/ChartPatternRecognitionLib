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
#include "PatternScanner.h"
#include "WedgeMatchValidationInfo.h"

// TODO Refactor to change the name of this class to UpperLowerTrendlinePatternScanner (or similar),
// since derived classes can scan anything from a flat base to a rising/falling/symetric wedge.
// WedgeScannerEngine is a scanner for a category of patterns where 2 trend lines come together in an
// apex. This can include descending triangle (downward upper trend line, flat lower trend line),
// ascending triangle (flat upper trend line, upward sloping lower trend line), or
// triangle (downward sloping upper trend line, upward sloping lower trend line).
class WedgeScannerEngine: public PatternScanner {

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

    // Validate the current pattern formation is valid (e.g., not too many
    // values outside the trend lines, not too heavily weighted to the top
    // or bottom of the pattern).
    double percClosingValsOutsideTrendLines(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    double ratioClosingValsAboveVsBelowMidpoint(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool percClosingValsOutsideTrendLinesWithinThreshold(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool ratioAboveVsBelowMidpointWithinThreshold(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool allClosingValsWithinThresholdOutsideTrendLines(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    virtual bool validWedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;


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

    static const double PERC_CLOSING_VALS_INSIDE_TRENDLINES_THRESHOLD;
    static const double RATIO_ABOVE_VS_BELOW_TRENDLINE_MIDPOINT_THRESHOLD;
    static const double MAX_DISTANCE_OUTSIDE_TRENDLINE_PERC_OF_CURR_DEPTH;

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
