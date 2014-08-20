/*
 * PatternMatchValidatorFactory.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#include <PatternMatchValidatorFactory.h>

PatternMatchValidatorFactory::PatternMatchValidatorFactory() {
}

PatternMatchValidatorPtr PatternMatchValidatorFactory::createValidator0() const
{
    PatternMatchVector previousMatches;
    return createValidator(previousMatches);
}


PatternMatchValidatorPtr PatternMatchValidatorFactory::createValidator1(const PatternMatchPtr &otherMatch) const
{
	PatternMatchVector previousMatches;
	previousMatches.push_back(otherMatch);
	return createValidator(previousMatches);
}

PatternMatchValidatorPtr PatternMatchValidatorFactory::createValidator2(const PatternMatchPtr &firstMatch,
		const PatternMatchPtr &secondMatch) const
{
	PatternMatchVector previousMatches;
	previousMatches.push_back(firstMatch);
	previousMatches.push_back(secondMatch);
	return createValidator(previousMatches);
}

PatternMatchValidatorPtr PatternMatchValidatorFactory::createValidator3(const PatternMatchPtr &firstMatch,
		const PatternMatchPtr &secondMatch,const PatternMatchPtr &thirdMatch) const
{
	PatternMatchVector previousMatches;
	previousMatches.push_back(firstMatch);
	previousMatches.push_back(secondMatch);
	previousMatches.push_back(thirdMatch);
	return createValidator(previousMatches);
}

PatternMatchValidatorPtr PatternMatchValidatorFactory::createValidator(const PatternMatchPtr &firstMatch,
		const PatternMatchPtr &secondMatch,const PatternMatchPtr &thirdMatch) const
{
	PatternMatchVector previousMatches;
	previousMatches.push_back(firstMatch);
	previousMatches.push_back(secondMatch);
	previousMatches.push_back(thirdMatch);
	return createValidator(previousMatches);
}


PatternMatchValidatorFactory::~PatternMatchValidatorFactory() {
}

