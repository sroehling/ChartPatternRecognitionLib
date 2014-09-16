#include "WedgeScanner.h"
#include <algorithm>
#include <math.h>

#define WEDGE_SCANNER_DEFAULT_MIN_PERIODS 8
#define WEDGE_SCANNER_DEFAULT_MAX_PERIODS 200
#define WEDGE_SCANNER_MAX_NUM_PERIODS_TO_INTERCEPT 100.0


WedgeScanner::WedgeScanner(const DoubleRange &upperTrendLineSlopeRange,
                           const DoubleRange &lowerTrendLineSlopeRange)
    : upperTrendLineSlopeRange_(upperTrendLineSlopeRange),
      lowerTrendLineSlopeRange_(lowerTrendLineSlopeRange),
      validPeriodRange_(WEDGE_SCANNER_DEFAULT_MIN_PERIODS,WEDGE_SCANNER_DEFAULT_MAX_PERIODS)
{
}


bool WedgeScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
    if(!(upperTrendLineSlopeRange_.valueWithinRange(upperTrendLine->slope()) &&
            lowerTrendLineSlopeRange_.valueWithinRange(lowerTrendLine->slope())))
    {
        return false;
    }

    // For wedges, both the upper and lower trend-lines will be facing upward or downward, respectively.
    // However, they still must "point toward each other" (i.e., not a megaphone type shape), such
    // that they intercept somewhere after the 2 pivot points.
    if(!interceptAfter2ndLowerAndUpperPivot(upperTrendLine,lowerTrendLine))
    {
        return false;
    }

    if(WedgeScannerEngine::numPeriodsToIntercept(upperTrendLine,lowerTrendLine)
            > WEDGE_SCANNER_MAX_NUM_PERIODS_TO_INTERCEPT)
    {
        // The lines must converge relatively quickly, or else they're more or less parallel and
        // the pattern looks more like a channel.
        return false;
    }

    if(!pivotsInterleaved(upperTrendLine,lowerTrendLine))
    {
        return false;
    }


    return true;
}

unsigned int WedgeScanner::minPatternPeriods(const ChartSegmentPtr &,
        const ChartSegmentPtr &) const
{
    return validPeriodRange_.minVal();
}

unsigned int WedgeScanner::maxPatternPeriods(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
    unsigned int interceptPers = floor(WedgeScannerEngine::numPeriodsToIntercept(upperTrendLine,lowerTrendLine));

    // Pattern length cannot go beyond whatever the intercept point is between the upper and lower trendlines.
    return interceptPers<validPeriodRange_.maxVal()?interceptPers:validPeriodRange_.maxVal();
}

