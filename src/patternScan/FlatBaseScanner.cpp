#include "FlatBaseScanner.h"
#include "WedgeScanner.h"
#include "DoubleRange.h"
#include "FlatBasePatternMatch.h"

#define FLAT_BASE_SCANNER_DEFAULT_MIN_PERIODS 8
#define FLAT_BASE_SCANNER_DEFAULT_MAX_PERIODS 200

FlatBaseScanner::FlatBaseScanner(const UnsignedIntRange &validPeriodRange)
    : validPeriodRange_(validPeriodRange)
{
}

FlatBaseScanner::FlatBaseScanner()
    : validPeriodRange_(UnsignedIntRange(FLAT_BASE_SCANNER_DEFAULT_MIN_PERIODS,FLAT_BASE_SCANNER_DEFAULT_MAX_PERIODS))
{
}

bool FlatBaseScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine) const
{
    DoubleRange validSlopeRange = WedgeScanner::FLAT_SLOPE_RANGE;

    if(!(validSlopeRange.valueWithinRange(upperTrendLine->slope()) &&
            validSlopeRange.valueWithinRange(lowerTrendLine->slope())))
    {
        return false;
    }

    if(!pivotsInterleaved(upperTrendLine,lowerTrendLine))
    {
        return false;
    }

    return true;
}

unsigned int FlatBaseScanner::minPatternPeriods(const ChartSegmentPtr &,
        const ChartSegmentPtr &) const
{
    return validPeriodRange_.minVal();
}

unsigned int FlatBaseScanner::maxPatternPeriods(const ChartSegmentPtr &, const ChartSegmentPtr &) const
{
    return validPeriodRange_.maxVal();
}

PatternMatchPtr FlatBaseScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        PatternMatchPtr patternMatch =  PatternMatchPtr(new FlatBasePatternMatch(wedgeMatchValidationInfo));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
    }

    return PatternMatchPtr(); // NULL (smart) pointer

}


