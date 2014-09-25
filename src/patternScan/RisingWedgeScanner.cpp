#include "RisingWedgeScanner.h"

#include "RisingWedgePatternMatch.h"

RisingWedgeScanner::RisingWedgeScanner()
    : WedgeScanner(WedgeScannerEngine::UPTREND_SLOPE_RANGE,WedgeScannerEngine::UPTREND_SLOPE_RANGE)
{
}


PatternMatchPtr RisingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RisingWedgePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RisingWedgePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }

    if(wedgeMatchValidationInfo.incompletePatternMatch())
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RisingWedgePatternMatch(wedgeMatchValidationInfo));
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
