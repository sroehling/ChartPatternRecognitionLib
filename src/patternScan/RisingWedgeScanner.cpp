#include "RisingWedgeScanner.h"
#include "RisingWedgePatternMatch.h"

RisingWedgeScanner::RisingWedgeScanner()
    : WedgeScanner(WedgeScanner::FLAT_SLOPE_RANGE,WedgeScanner::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr RisingWedgeScanner::findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RisingWedgePatternMatch(wedgeSeg,
                    wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
