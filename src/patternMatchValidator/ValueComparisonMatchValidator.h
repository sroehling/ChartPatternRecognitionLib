/*
 * LessThanEqualMatchValidator.h
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#ifndef LESSTHANEQUALMATCHVALIDATOR_H_
#define LESSTHANEQUALMATCHVALIDATOR_H_

#include <PatternMatchValidator.h>
#include "PatternMatchValueRef.h"
#include "ValueComparator.h"

class ValueComparisonMatchValidator: public PatternMatchValidator {
private:
	PatternMatchValueRefPtr lhsValRef_;
	PatternMatchValueRefPtr rhsValRef_;
	ValueComparatorPtr comparator_;
public:
	ValueComparisonMatchValidator(const PatternMatchValueRefPtr &lhsValRef,
			const PatternMatchValueRefPtr &rhsValRef,
			const ValueComparatorPtr &comparator);

	virtual bool validPattern(const PatternMatch &candidateMatch);

	virtual ~ValueComparisonMatchValidator() {}
};

#endif /* LESSTHANEQUALMATCHVALIDATOR_H_ */
