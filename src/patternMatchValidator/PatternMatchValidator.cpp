/*
 * PatternMatchValidator.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PatternMatchValidator.h"

PatternMatchValidator::PatternMatchValidator() {
}

PatternMatchValidator::~PatternMatchValidator() {
}

PatternMatchListPtr PatternMatchValidator::filterMatches(const PatternMatchValidatorPtr &validator,
		const PatternMatchListPtr &unfilteredMatches)
{
	PatternMatchListPtr filteredMatches(new PatternMatchList());
	for(PatternMatchList::const_iterator matchIter = unfilteredMatches->begin();
			matchIter != unfilteredMatches->end(); matchIter++)
	{
		if(validator->validPattern(**matchIter))
		{
			filteredMatches->push_back(*matchIter);
		}
	}
	return filteredMatches;
}

