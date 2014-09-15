#include "DescendingTriangleScanner.h"
#include "DescendingTrianglePatternMatch.h"

DescendingTriangleScanner::DescendingTriangleScanner()
    : TriangleScanner(TriangleScanner::DOWNTREND_SLOPE_RANGE,TriangleScanner::FLAT_SLOPE_RANGE)
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

    return PatternMatchPtr(); // NULL (smart) pointer

}

