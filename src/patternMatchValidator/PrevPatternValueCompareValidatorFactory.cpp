#include "PrevPatternValueCompareValidatorFactory.h"
#include "ValueComparisonMatchValidator.h"

PrevPatternValueCompareValidatorFactory::PrevPatternValueCompareValidatorFactory(
        const ValueComparatorPtr &comparison,
        const PatternMatchValueRefPtr &compareValueRef)
    : comparison_(comparison),
      compareValueRef_(compareValueRef)
{


}


PatternMatchValidatorPtr PrevPatternValueCompareValidatorFactory::createValidator(
        const PatternMatchVector &previousMatches) const
{
    // This validator always compares against the first previous match.
    assert(previousMatches.size() > 0);
    PatternMatchPtr otherMatch = previousMatches[0];

    double otherVal = compareValueRef_->patternMatchVal(*otherMatch);
    PatternMatchValueRefPtr otherValueRef(new FixedPatternMatchValueRef(otherVal));

    PatternMatchValidatorPtr comparisonValidator(
            new ValueComparisonMatchValidator(compareValueRef_,otherValueRef,comparison_));

    return comparisonValidator;
}
