/*
 * PatternMatchValidatorCreationHelper.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
#include <PatternMatchValidatorCreationHelper.h>
#include "ANDPatternMatchValidator.h"
#include "PatternMatchValueRef.h"
#include "PeriodValueRef.h"
#include "DoubleRange.h"

namespace patternMatchValidatorCreationHelper
{


PatternMatchValidatorPtr minDepthPercentValidator(double minDepthPerc)
{
	assert(DoubleRange(0.0,100.0).valueWithinRange(minDepthPerc));

	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
	PatternMatchValueRefPtr depthVal(new DepthPercentPatternMatchValueRef());
	PatternMatchValueRefPtr minDepthVal(new FixedPatternMatchValueRef(minDepthPerc));
	PatternMatchValidatorPtr minDepthValidator(
			new ValueComparisonMatchValidator(depthVal,minDepthVal,greaterEqualCompare));
	return minDepthValidator;
}

PatternMatchValidatorPtr maxDepthPercentValidator(double maxDepthPerc)
{
	assert(DoubleRange(0.0,100.0).valueWithinRange(maxDepthPerc));

	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr maxDepthVal(new FixedPatternMatchValueRef(maxDepthPerc));
	PatternMatchValueRefPtr depthVal(new DepthPercentPatternMatchValueRef());
	PatternMatchValidatorPtr maxDepthValidator(
			new ValueComparisonMatchValidator(depthVal,maxDepthVal,lessEqualCompare));
	return maxDepthValidator;
}


PatternMatchValidatorPtr depthWithinRangeValidator(const DoubleRange &minMaxDepthPerc)
{
	assert(minMaxDepthPerc.positiveVals());

	PatternMatchValidatorPtr minDepth = minDepthPercentValidator(minMaxDepthPerc.minVal());
	PatternMatchValidatorPtr maxDepth = maxDepthPercentValidator(minMaxDepthPerc.maxVal());

	PatternMatchValidatorList andList;
	andList.push_back(minDepth);
	andList.push_back(maxDepth);

	PatternMatchValidatorPtr depthInRange(new ANDPatternMatchValidator(andList));
	return depthInRange;
}

PatternMatchValidatorPtr lowerLowValidator(const PatternMatchPtr &compareWith)
{
	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr compareWithLow(new FixedPatternMatchValueRef(compareWith->lowestLow()));
	PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());
	PatternMatchValidatorPtr lowerLowValidator(
			new ValueComparisonMatchValidator(validationLowRef,compareWithLow,lessEqualCompare));
	return lowerLowValidator;
}

static PatternMatchValueRefPtr lastHighValueRef()
{
	// Reference the high value for the last value in the pattern match.
	PeriodValueRefPtr highValueRef(new HighPeriodValueRef());
	PatternMatchValueRefPtr lastHighRef(new LastPeriodValPatternMatchValueRef(highValueRef));
	return lastHighRef;
}


static PatternMatchValueRefPtr firstHighValueRef()
{
	// Reference the high value for the last value in the pattern match.
	PeriodValueRefPtr highValueRef(new HighPeriodValueRef());
	PatternMatchValueRefPtr firstHighRef(new FirstPeriodValPatternMatchValueRef(highValueRef));
	return firstHighRef;
}

PatternMatchValidatorPtr highestHighBelowLastHigh()
{
    PatternMatchValueRefPtr lastHigh = lastHighValueRef();
    PatternMatchValueRefPtr highestHigh(new HighestHighPatternMatchValueRef());
    ValueComparatorPtr lessThanEqualCompare(new LessThanEqualValueComparator());
    PatternMatchValidatorPtr lessThanLastHigh(
            new ValueComparisonMatchValidator(highestHigh,lastHigh,lessThanEqualCompare));

    return lessThanLastHigh;

}

PatternMatchValidatorPtr highestCloseBelowFirstHigh()
{
    PatternMatchValueRefPtr firstHigh = firstHighValueRef();

    PeriodValueRefPtr closeValRef(new ClosePeriodValueRef());
    PatternMatchValueRefPtr highestClose(new HighestValPatternMatchValueRef(closeValRef));

    ValueComparatorPtr lessThanEqualCompare(new LessThanEqualValueComparator());

    PatternMatchValidatorPtr lessThanFirstHigh(
            new ValueComparisonMatchValidator(highestClose,firstHigh,lessThanEqualCompare));

    return lessThanFirstHigh;

}


PatternMatchValidatorPtr lastHighAboveFixedValue(double thresholdValue)
{
	assert(thresholdValue >= 0.0);
	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());

	PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(thresholdValue));

	PatternMatchValidatorPtr aboveThresholdValidator(
			new ValueComparisonMatchValidator(lastHighValueRef(),thresholdValRef,greaterEqualCompare));
	return aboveThresholdValidator;
}

PatternMatchValidatorPtr lastHighAboveFirstHigh()
{
	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
	PatternMatchValidatorPtr aboveFirstHigh(
			new ValueComparisonMatchValidator(lastHighValueRef(),firstHighValueRef(),greaterEqualCompare));
	return aboveFirstHigh;
}

PatternMatchValidatorPtr lowAbovePercDepthOfOtherPattern(const PatternMatchPtr &otherPattern, double percDepthOther)
{
    ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
    PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());

    assert(percDepthOther >= 0.0);
    assert(percDepthOther <= 1.0);

    double firstHigh = otherPattern->firstValue().high();
    double lowestLow = otherPattern->lowestLow();
    assert(firstHigh >= lowestLow);
    double depth = firstHigh-lowestLow;

    double thresholdValue = firstHigh-(depth * percDepthOther);
    PatternMatchValueRefPtr thresholdValRef(new FixedPatternMatchValueRef(thresholdValue));

    BOOST_LOG_TRIVIAL(debug) << "lowAbovePercDepthOfOtherPattern: low threshold: " << thresholdValue;


    PatternMatchValidatorPtr aboveDepthOther(
            new ValueComparisonMatchValidator(validationLowRef,thresholdValRef,greaterEqualCompare));

    return aboveDepthOther;

}

}
