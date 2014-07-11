/*
 * CompositePatternMatchValidatorFactory.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#include <CompositePatternMatchValidatorFactory.h>
#include "StaticPatternMatchValidatorFactory.h"
#include "ANDPatternMatchValidator.h"

CompositePatternMatchValidatorFactory::CompositePatternMatchValidatorFactory() {
}

void CompositePatternMatchValidatorFactory::addFactory(const PatternMatchValidatorFactoryPtr &factory)
{
	factories_.push_back(factory);
}

void CompositePatternMatchValidatorFactory::addStaticValidator(const PatternMatchValidatorPtr &validator)
{
	PatternMatchValidatorFactoryPtr staticFactory(new StaticPatternMatchValidatorFactory(validator));
	factories_.push_back(staticFactory);
}

PatternMatchValidatorPtr CompositePatternMatchValidatorFactory::createValidator(const PatternMatchPtr &otherMatch) const
{
	PatternMatchValidatorList validators;
	for(PatternMatchValidatorFactoryList::const_iterator factoryIter = factories_.begin();
			factoryIter != factories_.end(); factoryIter++)
	{
		validators.push_back((*factoryIter)->createValidator(otherMatch));
	}
	return PatternMatchValidatorPtr(new ANDPatternMatchValidator(validators));
}

CompositePatternMatchValidatorFactory::~CompositePatternMatchValidatorFactory() {


}

