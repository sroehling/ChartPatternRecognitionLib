#include "FallingWedgeScanner.h"
#include "FallingWedgePatternMatch.h"

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScanner::DOWNTREND_SLOPE_RANGE,WedgeScanner::FLAT_SLOPE_RANGE)
{
}

PatternMatchPtr FallingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FallingWedgePatternMatch(wedgeSeg,
                  wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}

