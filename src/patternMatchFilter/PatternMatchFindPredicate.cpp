/*
 * PatternMatchFindPredicate.cpp
 *
 *  Created on: Jul 23, 2014
 *      Author: sroehling
 */

#include <PatternMatchFindPredicate.h>

bool LowestLowAfterTime::operator() (const PatternMatchPtr &patternMatch) const
{
	return patternMatch->lowestLowTime() > comparisonTime_;
}
