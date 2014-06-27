/*
 * PatternLengthAtLeast.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#include <PatternLengthAtLeast.h>

PatternLengthAtLeast::PatternLengthAtLeast(unsigned int minPeriods)
: minPeriods_(minPeriods)
{
	assert(minPeriods > 0);

}

bool PatternLengthAtLeast::validPattern(const PatternMatch &candidateMatch)
{
	return (candidateMatch.numPeriods() >= minPeriods_)?true:false;
}

PatternLengthAtLeast::~PatternLengthAtLeast() {
}

