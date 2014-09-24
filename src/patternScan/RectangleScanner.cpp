#include "RectangleScanner.h"
#include "TriangleScanner.h"
#include "DoubleRange.h"
#include "RectanglePatternMatch.h"

#define FLAT_BASE_SCANNER_DEFAULT_MIN_PERIODS 15
#define FLAT_BASE_SCANNER_DEFAULT_MAX_PERIODS 200

RectangleScanner::RectangleScanner(const UnsignedIntRange &validPeriodRange)
    : validPeriodRange_(validPeriodRange)
{
}

RectangleScanner::RectangleScanner()
    : validPeriodRange_(UnsignedIntRange(FLAT_BASE_SCANNER_DEFAULT_MIN_PERIODS,FLAT_BASE_SCANNER_DEFAULT_MAX_PERIODS))
{
}

bool RectangleScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine) const
{
    DoubleRange validSlopeRange = TriangleScanner::FLAT_SLOPE_RANGE;

    if(!WedgeScannerEngine::validTrendLines(upperTrendLine,lowerTrendLine))
    {
        return false;
    }

    // For starters, the trend-line slope for the upper and lower trendlines must be within
    // the acceptable ranges.
    if(!(WedgeScannerEngine::validTrendlinePercChangePerYear(validSlopeRange,upperTrendLine) &&
            WedgeScannerEngine::validTrendlinePercChangePerYear(validSlopeRange,lowerTrendLine)))
    {
        return false;
    }


    if(!pivotsInterleaved(upperTrendLine,lowerTrendLine))
    {
        return false;
    }

    return true;
}

unsigned int RectangleScanner::minPatternPeriods(const ChartSegmentPtr &,
        const ChartSegmentPtr &) const
{
    return validPeriodRange_.minVal();
}

unsigned int RectangleScanner::maxPatternPeriods(const ChartSegmentPtr &, const ChartSegmentPtr &) const
{
    return validPeriodRange_.maxVal();
}

PatternMatchPtr RectangleScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RectanglePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new RectanglePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }


    return PatternMatchPtr(); // NULL (smart) pointer

}


