/*
 * HigherLowPatternMatchValidatorFactory.h
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#ifndef HIGHERLOWPATTERNMATCHVALIDATORFACTORY_H_
#define HIGHERLOWPATTERNMATCHVALIDATORFACTORY_H_

#include <PatternMatchValidatorFactory.h>

class HigherLowPatternMatchValidatorFactory: public PatternMatchValidatorFactory {
public:
	HigherLowPatternMatchValidatorFactory();
	virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;
	virtual ~HigherLowPatternMatchValidatorFactory();
};

#endif /* HIGHERLOWPATTERNMATCHVALIDATORFACTORY_H_ */
