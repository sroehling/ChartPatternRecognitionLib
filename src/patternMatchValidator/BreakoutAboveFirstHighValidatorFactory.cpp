#include "BreakoutAboveFirstHighValidatorFactory.h"

#include "ValueComparator.h"
#include "PatternMatchValueRef.h"
#include "ValueComparisonMatchValidator.h"
#include "PeriodValueRef.h"
#include "ANDPatternMatchValidator.h"


BreakoutAboveFirstHighValidatorFactory::BreakoutAboveFirstHighValidatorFactory()
{
}


PatternMatchValidatorPtr BreakoutAboveFirstHighValidatorFactory::createValidator(const PatternMatchVector &previousMatches) const
{
    assert(previousMatches.size() >0);
    PatternMatchPtr prevMatch = previousMatches[0];

    ValueComparatorPtr greaterThanCompare(new GreaterThanValueComparator());
    ValueComparatorPtr lessThanCompare(new LessThanValueComparator());


    PatternMatchValueRefPtr firstHigh(new FixedPatternMatchValueRef(prevMatch->firstValue().high()));

    PeriodValueRefPtr closeValueRef(new ClosePeriodValueRef());

    PatternMatchValueRefPtr lastCloseRef(new LastPeriodValPatternMatchValueRef(closeValueRef));

    PatternMatchValueRefPtr secondToLastCloseRef(new SecondToLastPeriodValPatternMatchValueRef(closeValueRef));

    PatternMatchValidatorPtr closeAboveHighValidator(
            new ValueComparisonMatchValidator(lastCloseRef,firstHigh,greaterThanCompare));

    PatternMatchValidatorPtr secondToLastCloseBelowHighValidator(
            new ValueComparisonMatchValidator(secondToLastCloseRef,firstHigh,lessThanCompare));

    // Except for the last value, ensure none of the previous close values exceeds the first high.
    // This ensures breakout for the last value is preceded by another value which also goes
    // above the first high.
    ValueComparatorPtr lessCompare(new LessThanValueComparator());
    PatternMatchValueRefPtr highestCloseExceptLast(new HighestValExceptLastPatternMatchValueRef(closeValueRef));
    PatternMatchValidatorPtr highestCloseExceptLastBelowFirstHigh(
                new ValueComparisonMatchValidator(highestCloseExceptLast,firstHigh,lessCompare));

    PatternMatchValidatorList breakoutValidators;
    breakoutValidators.push_back(closeAboveHighValidator);
    breakoutValidators.push_back(secondToLastCloseBelowHighValidator);
    breakoutValidators.push_back(highestCloseExceptLastBelowFirstHigh);

    PatternMatchValidatorPtr upSideBreakoutValidator(new ANDPatternMatchValidator(breakoutValidators));


    return upSideBreakoutValidator;

}
