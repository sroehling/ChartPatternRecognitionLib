#include "PrevPatternDepthThreshold.h"
#include "ValueComparisonMatchValidator.h"

PrevPatternDepthThreshold::PrevPatternDepthThreshold(double depthThresholdMult,
                                                     const PatternMatchValueRefPtr &comparisonVal,
                                                    const ValueComparatorPtr &comparator)
    : depthThresholdMult_(depthThresholdMult),
    comparisonVal_(comparisonVal),
    comparator_(comparator)
{
    assert(depthThresholdMult_ >= 0.0);
    assert(depthThresholdMult <= 10.0); // sanity check
}


PatternMatchValidatorPtr PrevPatternDepthThreshold::createValidator(
        const PatternMatchVector &previousMatches) const
{
    // This validator always compares against the first previous match.
    assert(previousMatches.size() > 0);
    PatternMatchPtr otherMatch = previousMatches[0];

    double depth = otherMatch->depthPoints();
    double depthThreshold = otherMatch->highestHigh()-(depth * depthThresholdMult_);

    PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(depthThreshold));

    PatternMatchValidatorPtr compareWithThresholdValidator(
            new ValueComparisonMatchValidator(comparisonVal_,thresholdValRef,comparator_));
    return compareWithThresholdValidator;

}
