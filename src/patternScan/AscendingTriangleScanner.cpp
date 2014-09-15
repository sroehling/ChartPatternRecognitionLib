#include "AscendingTriangleScanner.h"
#include "AscendingTrianglePatternMatch.h"

AscendingTriangleScanner::AscendingTriangleScanner()
    : TriangleScanner(TriangleScanner::FLAT_SLOPE_RANGE,TriangleScanner::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr AscendingTriangleScanner::findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new AscendingTrianglePatternMatch(wedgeSeg,
                    wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}
