/*
 * PatternLengthLessThanEqual.cpp
 *
 *  Created on: Jun 27, 2014
 *      Author: sroehling
 */

#include <PatternLengthLessThanEqual.h>

PatternLengthLessThanEqual::PatternLengthLessThanEqual(unsigned int maxPeriods)
: maxPeriods_(maxPeriods)
{
	assert(maxPeriods > 0);
}

bool PatternLengthLessThanEqual::validPattern(const PatternMatch &candidateMatch)
{
	return (candidateMatch.numPeriods() <= maxPeriods_)?true:false;
}


PatternLengthLessThanEqual::~PatternLengthLessThanEqual() {
}

