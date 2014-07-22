/*
 * ANDPatternMatchValidator.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#include <ANDPatternMatchValidator.h>

ANDPatternMatchValidator::ANDPatternMatchValidator(const PatternMatchValidatorList &andList)
: andList_(andList)
{
}


bool ANDPatternMatchValidator::validPattern(const PatternMatch &candidateMatch)
{
	if(andList_.size() == 0)
	{
		return true;
	}
	else
	{
		for(PatternMatchValidatorList::const_iterator valIter = andList_.begin();
				valIter != andList_.end(); valIter++)
		{
			if(!(*valIter)->validPattern(candidateMatch))
			{
				return false;
			}
		}
		return true;
	}
}


ANDPatternMatchValidator::~ANDPatternMatchValidator() {
}

