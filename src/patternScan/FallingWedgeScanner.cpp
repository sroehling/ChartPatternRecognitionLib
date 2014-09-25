#include "FallingWedgeScanner.h"

#include "FallingWedgePatternMatch.h"

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScannerEngine::DOWNTREND_SLOPE_RANGE,WedgeScannerEngine::DOWNTREND_SLOPE_RANGE)
{
}


PatternMatchPtr FallingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FallingWedgePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FallingWedgePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }

    if(wedgeMatchValidationInfo.incompletePatternMatch())
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FallingWedgePatternMatch(wedgeMatchValidationInfo));
        return patternMatch;
    }


    return PatternMatchPtr(); // NULL (smart) pointer

}
