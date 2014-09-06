#include "PrevPatternValueRatioValidatorFactory.h"
#include "ValueComparator.h"
#include "ANDPatternMatchValidator.h"
#include "ValueComparisonMatchValidator.h"

PrevPatternValueRatioValidatorFactory::PrevPatternValueRatioValidatorFactory(const DoubleRange &validRatios,
            const PatternMatchValueRefPtr &ratioValueRef)
    : validRatios_(validRatios),
      ratioValueRef_(ratioValueRef)
{
    assert(validRatios_.positiveVals()); // ratios must be positive
}

PatternMatchValidatorPtr PrevPatternValueRatioValidatorFactory::createValidator(
        const PatternMatchVector &previousMatches) const
{
    // This validator always compares against the first previous match.
    assert(previousMatches.size() > 0);
    PatternMatchPtr otherMatch = previousMatches[0];

    double otherRatioVal = ratioValueRef_->patternMatchVal(*otherMatch);

    double minRatioThreshold = otherRatioVal*validRatios_.minVal();
    PatternMatchValueRefPtr minThresholdValueRef(new FixedPatternMatchValueRef(minRatioThreshold));
    ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
    PatternMatchValidatorPtr minRatioValidator(
            new ValueComparisonMatchValidator(ratioValueRef_,minThresholdValueRef,greaterEqualCompare));

    double maxRatioThreshold = otherRatioVal*validRatios_.maxVal();
    PatternMatchValueRefPtr maxThresholdValueRef(new FixedPatternMatchValueRef(maxRatioThreshold));
    ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
    PatternMatchValidatorPtr maxRatioValidator(
            new ValueComparisonMatchValidator(ratioValueRef_,maxThresholdValueRef,lessEqualCompare));

    PatternMatchValidatorList minAndMax;
    minAndMax.push_back(minRatioValidator);
    minAndMax.push_back(maxRatioValidator);

    return PatternMatchValidatorPtr(new ANDPatternMatchValidator(minAndMax));

}
