#include "LowerHighPatternMatchValidatorFactory.h"
#include "ValueComparator.h"
#include "PatternMatchValueRef.h"
#include "ValueComparisonMatchValidator.h"


LowerHighPatternMatchValidatorFactory::LowerHighPatternMatchValidatorFactory()
{
}


PatternMatchValidatorPtr LowerHighPatternMatchValidatorFactory::createValidator(const PatternMatchVector &previousMatches) const
{
    assert(previousMatches.size()==1); // only works when provided with 1 previous match.

    PatternMatchPtr prevMatch = previousMatches[0];

    ValueComparatorPtr lessThanCompare(new LessThanValueComparator());

    PatternMatchValueRefPtr prevHigh(new FixedPatternMatchValueRef(prevMatch->highestHigh()));

    PatternMatchValueRefPtr validationHighRef(new HighestHighPatternMatchValueRef());

    PatternMatchValidatorPtr lowerHighValidator(
            new ValueComparisonMatchValidator(validationHighRef,prevHigh,lessThanCompare));

    return lowerHighValidator;

}
