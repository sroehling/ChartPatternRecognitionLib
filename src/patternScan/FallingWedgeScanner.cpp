#include "FallingWedgeScanner.h"

#include "FallingWedgePatternMatch.h"
#include "DoubleRange.h"

#define FALLING_WEDGE_SCANNER_MIN_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO 0.1
#define FALLING_WEDGE_SCANNER_MAX_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO 0.5

FallingWedgeScanner::FallingWedgeScanner()
    : WedgeScanner(WedgeScannerEngine::DOWNTREND_SLOPE_RANGE,WedgeScannerEngine::DOWNTREND_SLOPE_RANGE)
{
}


PatternMatchPtr FallingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{

    if(!DoubleRange(FALLING_WEDGE_SCANNER_MIN_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO,
                    FALLING_WEDGE_SCANNER_MAX_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO).valueWithinRange(
                wedgeMatchValidationInfo.lastToFirstTrendlineDistanceRatio()))
    {
        return PatternMatchPtr(); // NULL (smart) pointer
    }


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
