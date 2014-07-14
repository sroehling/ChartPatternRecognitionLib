/*
 * HigherLowPatternMatchValidatorFactory.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#include <HigherLowPatternMatchValidatorFactory.h>
#include "ValueComparator.h"
#include "PatternMatchValueRef.h"

HigherLowPatternMatchValidatorFactory::HigherLowPatternMatchValidatorFactory() {
}

PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const
{
	assert(previousMatches.size()==1); // only works when provided with 1 previous match.

	PatternMatchPtr prevMatch = previousMatches[0];

	ValueComparatorPtr greaterThanEqualCompare(new GreaterThanEqualValueComparator());

	PatternMatchValueRefPtr prevLow(new FixedPatternMatchValueRef(prevMatch->lowestLow()));

	PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());

	PatternMatchValidatorPtr higherLowValidator(
			new ValueComparisonMatchValidator(validationLowRef,prevLow,greaterThanEqualCompare));
	return higherLowValidator;

}

HigherLowPatternMatchValidatorFactory::~HigherLowPatternMatchValidatorFactory() {
}

