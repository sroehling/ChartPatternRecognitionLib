#include "RisingWedgeScanner.h"

#include "RisingWedgePatternMatch.h"
#include "DoubleRange.h"

#define RISING_WEDGE_SCANNER_MIN_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO 0.1
#define RISING_WEDGE_SCANNER_MAX_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO 0.5

RisingWedgeScanner::RisingWedgeScanner()
    : WedgeScanner(WedgeScannerEngine::UPTREND_SLOPE_RANGE,WedgeScannerEngine::UPTREND_SLOPE_RANGE)
{
}


PatternMatchPtr RisingWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{

    if(!DoubleRange(RISING_WEDGE_SCANNER_MIN_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO,
                    RISING_WEDGE_SCANNER_MAX_LAST_TO_FIRST_TRENDLINE_DISTANCE_RATIO).valueWithinRange(
                wedgeMatchValidationInfo.lastToFirstTrendlineDistanceRatio()))
    {
        return PatternMatchPtr(); // NULL (smart) pointer
    }

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
