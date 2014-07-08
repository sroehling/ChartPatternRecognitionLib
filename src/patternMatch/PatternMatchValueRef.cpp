/*
 * PatternMatchValueRef.cpp
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#include <PatternMatchValueRef.h>

double LowestLowPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return match.lowestLow();
}


double DepthPercentPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return match.depthPercent();
}

double FixedPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return val_;
}
