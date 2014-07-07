/*
 * LessThanEqualMatchValidator.cpp
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#include <ValueComparisonMatchValidator.h>

ValueComparisonMatchValidator::ValueComparisonMatchValidator(const PatternMatchValueRefPtr &lhsValRef,
		const PatternMatchValueRefPtr &rhsValRef,
		const ValueComparatorPtr &comparator)
: lhsValRef_(lhsValRef),
  rhsValRef_(rhsValRef),
  comparator_(comparator)
{
}

bool ValueComparisonMatchValidator::validPattern(const PatternMatch &candidateMatch)
{
	return comparator_->compare(lhsValRef_->patternMatchVal(candidateMatch),
			rhsValRef_->patternMatchVal(candidateMatch));
}
