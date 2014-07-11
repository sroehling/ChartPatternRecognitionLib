/*
 * CompositePatternMatchValidatorFactory.h
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#ifndef COMPOSITEPATTERNMATCHVALIDATORFACTORY_H_
#define COMPOSITEPATTERNMATCHVALIDATORFACTORY_H_

#include "PatternMatchValidatorFactory.h"

class CompositePatternMatchValidatorFactory: public PatternMatchValidatorFactory {
private:
	PatternMatchValidatorFactoryList factories_;
public:
	CompositePatternMatchValidatorFactory();

	void addFactory(const PatternMatchValidatorFactoryPtr &factory);
	void addStaticValidator(const PatternMatchValidatorPtr &validator);

	virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const;

	virtual ~CompositePatternMatchValidatorFactory();
};

#endif /* COMPOSITEPATTERNMATCHVALIDATORFACTORY_H_ */
