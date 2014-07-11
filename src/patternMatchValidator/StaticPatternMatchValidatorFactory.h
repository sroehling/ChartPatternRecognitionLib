/*
 * StaticPatternMatchValidatorFactory.h
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#ifndef STATICPATTERNMATCHVALIDATORFACTORY_H_
#define STATICPATTERNMATCHVALIDATORFACTORY_H_

#include "PatternMatchValidatorFactory.h"

class StaticPatternMatchValidatorFactory: public PatternMatchValidatorFactory {
private:
	PatternMatchValidatorPtr staticValidator_;
public:
	StaticPatternMatchValidatorFactory(const PatternMatchValidatorPtr &staticValidator);

	virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;

	virtual ~StaticPatternMatchValidatorFactory();
};

#endif /* STATICPATTERNMATCHVALIDATORFACTORY_H_ */
