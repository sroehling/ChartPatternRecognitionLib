/*
 * ORPatternMatchValidator.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: sroehling
 */

#include <ORPatternMatchValidator.h>

ORPatternMatchValidator::ORPatternMatchValidator(const PatternMatchValidatorList &orList)
: orList_(orList)
{
}

bool ORPatternMatchValidator::validPattern(const PatternMatch &candidateMatch)
{
	if(orList_.size() == 0)
	{
		return true;
	}
	else
	{
		for(PatternMatchValidatorList::const_iterator valIter = orList_.begin();
				valIter != orList_.end(); valIter++)
		{
			if((*valIter)->validPattern(candidateMatch))
			{
				return true;
			}
		}
		return false;
	}
}



ORPatternMatchValidator::~ORPatternMatchValidator() {
	// TODO Auto-generated destructor stub
}

