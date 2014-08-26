#include "RisingWedgeScanner.h"
#include "RisingWedgePatternMatch.h"

RisingWedgeScanner::RisingWedgeScanner()
    : WedgeScanner(WedgeScanner::FLAT_SLOPE_RANGE,WedgeScanner::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr RisingWedgeScanner::findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    if (wedgeMatchValidationInfo.upperTrendLineBreakout())
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        return PatternMatchPtr(new RisingWedgePatternMatch(wedgeSeg,
                    wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
