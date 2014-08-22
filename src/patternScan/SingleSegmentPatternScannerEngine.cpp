#include "SingleSegmentPatternScannerEngine.h"
#include "ChartSegment.h"
#include "PatternMatch.h"


const UnsignedIntRange SingleSegmentPatternScannerEngine::DEFAULT_SCAN_LENGTH_RANGE(3,200);


SingleSegmentPatternScannerEngine::SingleSegmentPatternScannerEngine()
    : segmentLengthRange_(SingleSegmentPatternScannerEngine::DEFAULT_SCAN_LENGTH_RANGE)
{
}


PatternMatchListPtr SingleSegmentPatternScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    // matchingPatterns remains empty, unless one or patterns are matched
    PatternMatchListPtr matchingPatterns = PatternMatchListPtr(new PatternMatchList());

    unsigned int minRemSegmentLen = segmentLengthRange_.minVal();
    unsigned int maxRemSegmentLen = std::min((unsigned int)chartVals->numVals(),
                    segmentLengthRange_.maxVal());

    if(chartVals->numVals() < minRemSegmentLen)
    {
        // There's not enough values remaining to continue
        // matching, so return an empty list of matches.
        return matchingPatterns;
    }

    PatternMatchValidatorPtr candidateMatchValidator = validatorFactory_.createValidator0();

    // The for loop logic includes a '<=' since a splitPos for maxRemSegmentLen
    // will return all the remaining values.
    for(unsigned int splitPos = minRemSegmentLen; splitPos <= maxRemSegmentLen; splitPos++)
    {
        PeriodValSegmentPair segmentSplit = chartVals->split(splitPos);
        PeriodValSegmentPtr candidateSegment = segmentSplit.first;

        ChartSegmentPtr chartSeg(new ChartSegment(candidateSegment));
        PatternMatchPtr candidateMatch(new PatternMatch(chartSeg));

        if(candidateMatchValidator->validPattern(*candidateMatch))
        {
            matchingPatterns->push_back(candidateMatch);
        }


    }

    return matchingPatterns;
}
