#include "DescendingTriangleScanner.h"
#include "DescendingTrianglePatternMatch.h"

DescendingTriangleScanner::DescendingTriangleScanner()
    : TriangleScanner(WedgeScannerEngine::DOWNTREND_SLOPE_RANGE,WedgeScannerEngine::FLAT_SLOPE_RANGE)
{
}

PatternMatchPtr DescendingTriangleScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new DescendingTrianglePatternMatch(wedgeSeg,
                  wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch =  PatternMatchPtr(new DescendingTrianglePatternMatch(wedgeSeg,
                    wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}

