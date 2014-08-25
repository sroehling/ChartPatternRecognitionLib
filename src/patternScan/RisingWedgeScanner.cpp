#include "RisingWedgeScanner.h"
#include "RisingWedgePatternMatch.h"

RisingWedgeScanner::RisingWedgeScanner()
    : WedgeScanner(WedgeScanner::FLAT_SLOPE_RANGE,WedgeScanner::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr RisingWedgeScanner::findPatternMatch(const PeriodValSegmentPtr &chartVals,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine,
        const PeriodValCltn::iterator &currPerValIter) const
{
    if (upperTrendLineBreakout(chartVals,upperTrendLine,currPerValIter))
    {
        ChartSegmentPtr wedgeSeg = createWedgeSegment(chartVals,upperTrendLine,currPerValIter);
        return PatternMatchPtr(new RisingWedgePatternMatch(wedgeSeg,upperTrendLine,lowerTrendLine));
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
