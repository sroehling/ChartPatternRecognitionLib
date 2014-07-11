/*
 * StaticPatternMatchValidatorFactory.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#include <StaticPatternMatchValidatorFactory.h>

StaticPatternMatchValidatorFactory::StaticPatternMatchValidatorFactory(const PatternMatchValidatorPtr &staticValidator)
: staticValidator_(staticValidator)
{
}

PatternMatchValidatorPtr StaticPatternMatchValidatorFactory::createValidator(const PatternMatchPtr &otherMatch) const
{
	// staticValidor_ is not dependent on the otherMatch for validation,
	// just return the validator as is (hence the "static" in the class name).
	return staticValidator_;
}

StaticPatternMatchValidatorFactory::~StaticPatternMatchValidatorFactory() {
}

