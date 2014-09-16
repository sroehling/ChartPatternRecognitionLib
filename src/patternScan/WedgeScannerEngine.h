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
#include "DoubleRange.h"

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

    bool pivotsInterleaved(const ChartSegmentPtr &upperTrendLine,
                           const ChartSegmentPtr &lowerTrendLine) const;
    bool first2PivotsInLHSOfWedge(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;


    // The number of periods from the first pivot high to the intercept
    // of the upper and lower trend line. Only works when the upper and
    // lower trend line in fact have an intercept.
    double numPeriodsToIntercept(const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine) const;

    bool interceptAfter2ndLowerAndUpperPivot(const ChartSegmentPtr &upperTrendLine,
                        const ChartSegmentPtr &lowerTrendLine) const;

    // Validate the current pattern formation is valid (e.g., not too many
    // values outside the trend lines, not too heavily weighted to the top
    // or bottom of the pattern).
    double percClosingValsOutsideTrendLines(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    double ratioClosingValsAboveVsBelowMidpoint(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool percClosingValsOutsideTrendLinesWithinThreshold(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool ratioAboveVsBelowMidpointWithinThreshold(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    bool allPeriodValsWithinThresholdOutsideTrendLines(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo, const PeriodValueRef &perValRef,
            double distanceAboveOrBelowPercDepthThreshold) const;
    bool allClosingValsWithinThresholdOutsideTrendLines(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
    virtual bool validWedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;


	// Test if a completed pattern match is found at the location pointed to by
	// currPerValIter.
    virtual PatternMatchPtr findPatternMatch(
            const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const = 0;

public:
	WedgeScannerEngine();

    static const double PERC_CLOSING_VALS_INSIDE_TRENDLINES_THRESHOLD;
    static const double RATIO_ABOVE_VS_BELOW_TRENDLINE_MIDPOINT_THRESHOLD;
    static const double MAX_DISTANCE_OUTSIDE_TRENDLINE_PERC_OF_CURR_DEPTH;
    static const double MAX_HIGH_LOW_DISTANCE_OUTSIDE_TRENDLINE_PERC_OF_CURR_DEPTH;


    static const DoubleRange DOWNTREND_SLOPE_RANGE;
    static const DoubleRange UPTREND_SLOPE_RANGE;
    static const DoubleRange FLAT_SLOPE_RANGE;


    // TODO - Provide an interface which allows passing in a pre-configured list of pivot highs
    // and lows. When there are multiple calls to the wedge scanners, this would facilitate
    // not having to
	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
