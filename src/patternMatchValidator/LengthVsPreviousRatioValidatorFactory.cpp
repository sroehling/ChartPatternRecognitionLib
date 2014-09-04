#include "LengthVsPreviousRatioValidatorFactory.h"
#include "ValueComparisonMatchValidator.h"

#define LENGTH_VS_PREVIOUS_VALIDATION_MIN_RATIO_PREV_VS_CURR 2.0

LengthVsPreviousRatioValidatorFactory::LengthVsPreviousRatioValidatorFactory()
{
}

PatternMatchValidatorPtr LengthVsPreviousRatioValidatorFactory::createValidator(
        const PatternMatchVector &previousMatches) const
{
    // This validator always compares against the first previous match.
    assert(previousMatches.size() > 0);
    PatternMatchPtr otherMatch = previousMatches[0];

    double prevNumVals = (double)otherMatch->numPeriods();

    // Length must be at least 1/2.0 of previous
    double lengthThreshold = prevNumVals / LENGTH_VS_PREVIOUS_VALIDATION_MIN_RATIO_PREV_VS_CURR;

    PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(lengthThreshold));

    ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());

    PatternMatchValueRefPtr numPeriodsRef(new NumPeriodsPatternMatchValueRef());

    PatternMatchValidatorPtr compareWithThresholdValidator(
            new ValueComparisonMatchValidator(numPeriodsRef,thresholdValRef,greaterEqualCompare));

    return compareWithThresholdValidator;

}

