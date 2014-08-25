#include "FallingWedgeScanner.h"
#include "FallingWedgePatternMatch.h"

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScanner::DOWNTREND_SLOPE_RANGE,WedgeScanner::FLAT_SLOPE_RANGE)
{
}

PatternMatchPtr FallingWedgeScanner::findPatternMatch(const PeriodValSegmentPtr &chartVals,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine,
        const PeriodValCltn::iterator &currPerValIter) const
{
    if (upperTrendLineBreakout(chartVals,upperTrendLine,currPerValIter))
    {
        ChartSegmentPtr wedgeSeg = createWedgeSegment(chartVals,upperTrendLine,currPerValIter);
        return PatternMatchPtr(new FallingWedgePatternMatch(wedgeSeg,upperTrendLine,lowerTrendLine));
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}

