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

double HighestHighPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.highestHigh();
}


double DepthPercentPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return match.depthPercent();
}


double DepthPointsPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.depthPoints();
}

double NumPeriodsPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return (double)match.numPeriods();
}

double FixedPatternMatchValueRef::patternMatchVal(const PatternMatch &) const
{
	return val_;
}


LastPeriodValPatternMatchValueRef::LastPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinLastPeriodVal)
: valRefWithinLastPeriodVal_(valRefWithinLastPeriodVal)
{

}

double LastPeriodValPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return valRefWithinLastPeriodVal_->referencedVal(match.lastValue());
}

SecondToLastPeriodValPatternMatchValueRef::SecondToLastPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinPeriodVal)
    : valRefWithinPeriodVal_(valRefWithinPeriodVal)
{

}

double SecondToLastPeriodValPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return valRefWithinPeriodVal_->referencedVal(match.secondToLastValue());
}


FirstPeriodValPatternMatchValueRef::FirstPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinPeriodVal)
: valRefWithinPeriodVal_(valRefWithinPeriodVal)
{

}

double FirstPeriodValPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return valRefWithinPeriodVal_->referencedVal(match.firstValue());
}
