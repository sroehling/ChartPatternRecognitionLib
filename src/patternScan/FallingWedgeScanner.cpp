#include "FallingWedgeScanner.h"
#include "FallingWedgePatternMatch.h"

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScanner::DOWNTREND_SLOPE_RANGE,WedgeScanner::FLAT_SLOPE_RANGE)
{
}

PatternMatchPtr FallingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    if (wedgeMatchValidationInfo.upperTrendLineBreakout())
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        return PatternMatchPtr(new FallingWedgePatternMatch(wedgeSeg,
                  wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}

