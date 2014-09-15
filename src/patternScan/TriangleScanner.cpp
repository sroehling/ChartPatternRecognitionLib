/*
 * WedgeScanner.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>

#include <TriangleScanner.h>
#include "ChartSegment.h"

const DoubleRange TriangleScanner::UPTREND_SLOPE_RANGE(0.020,100.0);
const DoubleRange TriangleScanner::DOWNTREND_SLOPE_RANGE(-100.0,-0.020);
const DoubleRange TriangleScanner::FLAT_SLOPE_RANGE(-0.020,0.020);


TriangleScanner::TriangleScanner(const DoubleRange &upperTrendLineSlopeRange,
                           const DoubleRange &lowerTrendLineSlopeRange)
    : upperTrendLineSlopeRange_(upperTrendLineSlopeRange),
      lowerTrendLineSlopeRange_(lowerTrendLineSlopeRange)
{
	minPercDistanceToUpperLowerTrendlineIntercept_ = 0.6;
	minPercValsBetweenTrendlines_ = 0.85;
}

double TriangleScanner::numPeriodsToIntercept(const ChartSegmentPtr &upperTrendLine,const ChartSegmentPtr &lowerTrendLine) const
{
	double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();

	XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));
	double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;

    assert(numPeriodsToIntercept > 0.0);

	return numPeriodsToIntercept;
}

bool TriangleScanner::pivotsSpacedOut(const ChartSegmentPtr &upperTrendLine,
                       const ChartSegmentPtr &lowerTrendLine) const
{
    double numPerToIntercept = numPeriodsToIntercept(upperTrendLine,lowerTrendLine);
    assert(numPerToIntercept > 0.0);

    // If the distance between the pivot points doesn't cover a reasonable amount
    // of the pattern, the pattern will look "bunched up" in one area.
    double MIN_SPACING_PERC = 0.30;

    double pivotLowSpacing = lowerTrendLine->lastPeriodVal().pseudoXVal()-lowerTrendLine->firstPeriodVal().pseudoXVal();
    assert(pivotLowSpacing <= numPerToIntercept);
    if((pivotLowSpacing/numPerToIntercept) < MIN_SPACING_PERC)
    {
        return false;
    }

    double pivotHighSpacing = upperTrendLine->lastPeriodVal().pseudoXVal()-upperTrendLine->firstPeriodVal().pseudoXVal();
    if(pivotHighSpacing >= numPerToIntercept)
    {
        BOOST_LOG_TRIVIAL(debug) << "ERROR: WedgeScanner: "
                << " pivot high space: " << pivotHighSpacing
                << " intercept periods: " << numPerToIntercept << std::endl;

    }
    assert(pivotHighSpacing <= numPerToIntercept);
    if(pivotHighSpacing > numPerToIntercept)
    {
        BOOST_LOG_TRIVIAL(debug) << "WedgeScanner: "
                << " pivot high space: " << pivotHighSpacing
                << " intercept periods: " << numPerToIntercept << std::endl;

    }
    if((pivotHighSpacing/numPerToIntercept) < MIN_SPACING_PERC)
    {
        return false;
    }

    return true;
}

bool TriangleScanner::interceptAfter2ndLowerAndUpperPivot(const ChartSegmentPtr &upperTrendLine,
                                                 const ChartSegmentPtr &lowerTrendLine) const
{
    if(lowerTrendLine->segmentEq()->slope() == upperTrendLine->segmentEq()->slope())
    {
        // never intercept
        return false;
    }

    // Check if the intercept occurs after the both the 1st and 2nd pivots "pseudo X" value
    // (i.e., the unique numerical value assigned for each PeriodVal's date). Otherwise,
    // we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
    // at some point, but not here), or the trend-lines intersect before the 2nd pivots have occured.

    XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));
    if(trendlineIntercept.x() > upperTrendLine->lastPeriodVal().pseudoXVal() &&
       trendlineIntercept.x() > lowerTrendLine->lastPeriodVal().pseudoXVal())
    {
        BOOST_LOG_TRIVIAL(debug) << "WedgeScanner::interceptAfter2ndLowerAndUpperPivot: "
                << " trend line intercept: " << trendlineIntercept.x()
                << " first pivot high: " << upperTrendLine->firstPeriodVal().pseudoXVal()
                << " last upper pivot high: " << upperTrendLine->lastPeriodVal().pseudoXVal()
                << " last upper pivot low:: " << lowerTrendLine->lastPeriodVal().pseudoXVal() << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}


bool TriangleScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{

    // For starters, the trend-line slope for the upper and lower trendlines must be within
    // the acceptable ranges.
    if(!(upperTrendLineSlopeRange_.valueWithinRange(upperTrendLine->slope()) &&
            lowerTrendLineSlopeRange_.valueWithinRange(lowerTrendLine->slope())))
    {
        return false;
    }

    // The first and last value of the upper and lower trend lines are the pivot highs and lows used to
    // define the trend lines. With this in mind, we don't want to consider a pattern valid if
    // there is not a pivot low before the second pivot high; in other words, the pattern is not
    // considered valid (i.e., well-balanced) if both the pivot lows occur after both the pivot highs.

    if(interceptAfter2ndLowerAndUpperPivot(upperTrendLine,lowerTrendLine))
    {
        if(!pivotsInterleaved(upperTrendLine,lowerTrendLine))
        {
            return false;
        }

        if(!pivotsSpacedOut(upperTrendLine,lowerTrendLine))
        {
            return false;
        }

        return true;

    }
    else
    {
        return false;
    }

}

unsigned int TriangleScanner::minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));
	double numPeriodsToIntercept = this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine);

	double minPeriods = floor(minPercDistanceToUpperLowerTrendlineIntercept_ * numPeriodsToIntercept);

	return minPeriods;
}

unsigned int TriangleScanner::maxPatternPeriods(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));

	unsigned int maxPeriods = floor(this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine));
	BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: "
			<< maxPeriods << std::endl;

	return maxPeriods;
}



TriangleScanner::~TriangleScanner() {
}

