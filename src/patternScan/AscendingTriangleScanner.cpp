#include "AscendingTriangleScanner.h"
#include "AscendingTrianglePatternMatch.h"

AscendingTriangleScanner::AscendingTriangleScanner()
    : TriangleScanner(WedgeScannerEngine::FLAT_SLOPE_RANGE,WedgeScannerEngine::UPTREND_SLOPE_RANGE)
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

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new AscendingTrianglePatternMatch(wedgeSeg,
                    wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }


    return PatternMatchPtr(); // NULL (smart) pointer

}
