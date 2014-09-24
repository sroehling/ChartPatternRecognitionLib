/*
 * PatternMatchValueRef.cpp
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#include <PatternMatchValueRef.h>
#include "MathHelper.h"

double LowestLowPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return match.lowestLow();
}

double HighestHighPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.highestHigh();
}


HighestValPatternMatchValueRef::HighestValPatternMatchValueRef(const PeriodValueRefPtr &valueRef)
    : valueRef_(valueRef)
{

}


double HighestValPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.matchSegment()->highestVal(*valueRef_);
}


HighestValExceptLastPatternMatchValueRef::HighestValExceptLastPatternMatchValueRef(const PeriodValueRefPtr &valueRef)
    : valueRef_(valueRef)
{

}


double HighestValExceptLastPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.matchSegment()->highestValExceptLast(*valueRef_);
}


double CalendarDaysLengthPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.matchSegment()->segmentSpanCalendarDays();
}


double DepthPercentPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
	return match.depthPercent();
}


double DepthPointsPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return match.depthPoints();
}

double DepthPointsFirstHighLowestLowPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    double depthHighLow = match.firstValue().high()-match.lowestLow();
    assert(depthHighLow > 0.0);
    return depthHighLow;
}

double DepthPercentFirstHighLowestLowPatternMatchValueRef::patternMatchVal(const PatternMatch &match) const
{
    return MathHelper::absRelPercentVal(match.lowestLow(),match.firstValue().high());
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
