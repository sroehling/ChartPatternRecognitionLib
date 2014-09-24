/*
 * PeriodValSegment.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <PeriodValSegment.h>
#include <iterator>
#include "MathHelper.h"
#include "PeriodValueRef.h"
#include "TimeHelper.h"


PeriodValCltn::iterator PeriodValSegment::highestValIter(const PeriodValueRef &perValRef,
    const PeriodValCltn::iterator &beginRange, const PeriodValCltn::iterator &endRange) const
{
    double maxVal = MathHelper::minDouble();
    PeriodValCltn::iterator highValIter = beginRange;
    for(PeriodValCltn::iterator perValIter = beginRange;
            perValIter != endRange; perValIter++)
    {
        double perVal = perValRef.referencedVal(*perValIter);
        if(perVal > maxVal)
        {
            maxVal = perVal;
            highValIter = perValIter;
        }
    }
    return highValIter;
}


PeriodValCltn::iterator PeriodValSegment::highestValIter(const PeriodValueRef &perValRef) const
{
    return highestValIter(perValRef,segBegin(), segEnd());
}

double PeriodValSegment::highestVal(const PeriodValueRef &perValRef) const
{
    PeriodValCltn::iterator valIter = highestValIter(perValRef);
    return perValRef.referencedVal(*valIter);
}

double PeriodValSegment::highestValExceptLast(const PeriodValueRef &perValRef) const
{
    assert(numVals() >= 2);
    PeriodValCltn::iterator valIter = highestValIter(perValRef,segBegin(),lastValIter());
    return perValRef.referencedVal(*valIter);
}


PeriodValCltn::iterator PeriodValSegment::lowestValIter(const PeriodValueRef &perValRef) const
{
    double minVal = MathHelper::maxDouble();
    PeriodValCltn::iterator lowValIter = segBegin();
    for(PeriodValCltn::iterator perValIter = segBegin();
            perValIter != segEnd(); perValIter++)
    {
        double perVal = perValRef.referencedVal(*perValIter);
        if((*perValIter).low() < minVal)
        {
            minVal = perVal;
            lowValIter = perValIter;
        }
    }
    return lowValIter;
}

double PeriodValSegment::lowestVal(const PeriodValueRef &perValRef) const
{
    PeriodValCltn::iterator valIter = lowestValIter(perValRef);
    return perValRef.referencedVal(*valIter);
}


void PeriodValSegment::initHighestHighValIter()
{
    highestHighValIter_ = highestValIter(HighPeriodValueRef());
}

void PeriodValSegment::initLowestLowValIter()
{
    lowestLowValIter_ = lowestValIter(LowPeriodValueRef());
}

bool PeriodValSegment::validPeriodValIndices() const
{

	if(segBegin() == segEnd())
	{
		return true;
	}
	PeriodValCltn::const_iterator checkBeginIter = segBegin();
	unsigned int lastIndex = (*segBegin()).perValIndex();
	checkBeginIter++;
	unsigned int segPos = 1;
	for(PeriodValCltn::const_iterator perValIter = checkBeginIter;
			perValIter != segEnd(); perValIter++)
	{
		unsigned int currIndex = (*perValIter).perValIndex();
		if((currIndex-1) != lastIndex)
		{
			return false;
		}
		lastIndex = currIndex;
		segPos++;
	}
	return true;
}

void PeriodValSegment::initSegments(const PeriodValCltn::iterator &segBegin,
		const PeriodValCltn::iterator &segEnd)
{
	segBegin_ = segBegin;
	segEnd_ = segEnd;

	// segEnd needs to be after segStart (or at least
	// point to the same location.
	startPos_ = std::distance(perValCltn_->begin(),segBegin);
	endPos_ = std::distance(perValCltn_->begin(),segEnd);

	assert((endPos_-startPos_) >= 0);
	assert(numVals() <= perValCltn_->size());

	initHighestHighValIter();
	initLowestLowValIter();
	assert(validPeriodValIndices());

}

PeriodValSegment::PeriodValSegment(const PeriodValCltnPtr &perValCltn,
		const PeriodValCltn::iterator &segBegin,
		const PeriodValCltn::iterator &segEnd)
: perValCltn_(perValCltn)
{
	initSegments(segBegin,segEnd);
}

PeriodValSegment::PeriodValSegment(const PeriodValCltnPtr &perValCltn)
: perValCltn_(perValCltn)
{
	initSegments(perValCltn->begin(),perValCltn->end());
	assert(numVals() == perValCltn->size());
}

PeriodValSegmentPtr  PeriodValSegment::moveSegBeginToPrev() const
{
	assert(startPos_ > 0);
	PeriodValCltn::iterator newSegBegin = segBegin_;
	newSegBegin--;
	return PeriodValSegmentPtr(new PeriodValSegment(perValCltn_,newSegBegin,segEnd_));
}

PeriodValSegmentPair PeriodValSegment::split(unsigned int splitPos) const
{
	assert(splitPos >= 0);
	assert(splitPos <= numVals());

	PeriodValCltn::iterator splitIter = segBegin_;
	std::advance(splitIter,splitPos);

	PeriodValSegmentPtr beforeSplit(new PeriodValSegment(perValCltn_,segBegin_,splitIter));
	PeriodValSegmentPtr afterSplit(new PeriodValSegment(perValCltn_,splitIter,segEnd_));

	return PeriodValSegmentPair(beforeSplit,afterSplit);
}

PeriodValSegmentPtr PeriodValSegment::spliceRange(unsigned int startPos, unsigned int endPos) const
{
	assert(endPos >= startPos);
	PeriodValSegmentPair beforeAfterSplice = split(startPos);
	PeriodValSegmentPair spliceAndSuffice = beforeAfterSplice.second->split(endPos-startPos);
	return spliceAndSuffice.first;
}

PeriodValSegmentPtr PeriodValSegment::trailingVals() const
{
	PeriodValSegmentPtr trailing(new PeriodValSegment(perValCltn_,segEnd_,perValCltn_->end()));
	return trailing;
}

PeriodValSegmentPtr PeriodValSegment::trailingValsWithLastVal() const
{
	PeriodValCltn::iterator trailingStart = segEnd_;
	if(segEnd_ != perValCltn_->begin())
	{
		trailingStart--;
	}
	PeriodValSegmentPtr trailing(new PeriodValSegment(perValCltn_,trailingStart,perValCltn_->end()));
	return trailing;
}


PeriodValSegmentPtr PeriodValSegment::trailingValsWithLastVal(unsigned int maxVals) const
{
    assert(maxVals > 0);
    PeriodValCltn::iterator trailingStart = segEnd_;
    if(segEnd_ != perValCltn_->begin())
    {
        trailingStart--;
    }

    unsigned int valsIncluded = 0;
    PeriodValCltn::iterator trailingEnd = trailingStart;
    while((valsIncluded < maxVals) && (trailingEnd != perValCltn_->end()))
    {
        trailingEnd++;
        valsIncluded++;
    }

    PeriodValSegmentPtr trailing(new PeriodValSegment(perValCltn_,trailingStart,trailingEnd));

    return trailing;
}


unsigned int PeriodValSegment::numVals() const
{
	return endPos_- startPos_;
}

double PeriodValSegment::averageCalendarMsecPerPeriod() const
{
    using namespace boost::posix_time;
    using namespace timeHelper;

    PeriodValCltnPtr overallPerValCltn = this->perValCltn();
    assert(overallPerValCltn->size() > 1);

    ptime endPeriodDataTime = overallPerValCltn->back().periodTime();
    ptime startPeriodDataTime = overallPerValCltn->front().periodTime();
    assert(endPeriodDataTime > startPeriodDataTime);

    double overallDiffMsec = timeHelper::timeDifferenceMsec(startPeriodDataTime,endPeriodDataTime);

    double msecPerPeriod = overallDiffMsec/((double)overallPerValCltn->size()-1.0);

    return msecPerPeriod;

}

double PeriodValSegment::averageCalendarDaysPerPeriod() const
{
    double msecPerPeriod = averageCalendarMsecPerPeriod();

    assert(msecPerPeriod > 0.0);

    double secsPerPeriod = msecPerPeriod / 1000.0;

    double daysPerPeriod = secsPerPeriod / 86400.0;

    return daysPerPeriod;
}

double PeriodValSegment::averageCalendarMonthsPerPeriod() const
{
    double avgDaysPerMonthIncludingLeapYear = 365.25 / 12.0;

    double monthsPerPeriod = averageCalendarDaysPerPeriod()/avgDaysPerMonthIncludingLeapYear;

    return monthsPerPeriod;
}

double PeriodValSegment::averageCalendarPeriodsPerYear() const
{
    double daysPerYear = 365.25;

    return daysPerYear / averageCalendarDaysPerPeriod();

}

unsigned int PeriodValSegment::segmentSpanPeriods() const
{
    unsigned int startSegmentPeriod = firstVal().perValIndex();
    unsigned int endSegmentPeriod = lastVal().perValIndex();
    assert(endSegmentPeriod > startSegmentPeriod);
    unsigned int segmentSpan = endSegmentPeriod-startSegmentPeriod;
    return segmentSpan;
}

double PeriodValSegment::segmentSpanCalendarMonths() const
{
    return (double)(segmentSpanPeriods()) * averageCalendarMonthsPerPeriod();
}

double PeriodValSegment::perPeriodPercChangeAlongLine(const LinearEquation &line) const
{
    // Compute the overall percent change from the start to the end of the segment
    double startYVal = line.yVal(firstVal().pseudoXVal());
    assert(startYVal > 0.0);
    double endYVal = line.yVal(lastVal().pseudoXVal());
    double startEndDiffPerc = (endYVal - startYVal)/startYVal;

    // Total number of periods from the start to the end of segment
    unsigned int startSegmentPeriod = firstVal().perValIndex();
    unsigned int endSegmentPeriod = lastVal().perValIndex();
    assert(endSegmentPeriod > startSegmentPeriod);
    unsigned int startEndNumPeriods = endSegmentPeriod - startSegmentPeriod;

    // Using interest rate conversion, determine the per-period percent change
    double perPeriodPercChange = std::pow(1.0+startEndDiffPerc,1.0/(double)startEndNumPeriods)-1.0;

    return perPeriodPercChange;
}

double PeriodValSegment::percChangePerYearAlongLine(const LinearEquation &line) const
{
    double yearlyPercChange = std::pow(1.0+perPeriodPercChangeAlongLine(line),averageCalendarPeriodsPerYear())-1.0;

    return yearlyPercChange;
}


const PeriodVal &PeriodValSegment::firstVal() const
{
	assert(numVals() > 0);
	return (*segBegin_);

}

const PeriodValCltn::iterator PeriodValSegment::lastValIter() const
{
	assert(numVals() > 0);
	PeriodValCltn::iterator lastValIter = segEnd_;
	lastValIter--;
	return lastValIter;
}

const PeriodVal &PeriodValSegment::lastVal() const
{
	return (*lastValIter());
}

const PeriodVal PeriodValSegment::highestHighVal() const
{
	assert(numVals()>0); // there's only a highest high if there is at least 1 value
	return (*highestHighValIter_);
}

double PeriodValSegment::highestHigh() const
{
	return this->highestHighVal().high();
}

const PeriodVal PeriodValSegment::lowestLowVal() const
{
	assert(numVals()>0); // there's only a lowest low if there is at least 1 value
	return (*lowestLowValIter_);
}


double PeriodValSegment::lowestLow() const
{
	return this->lowestLowVal().low();
}

double PeriodValSegment::depthPoints() const
{
	double depth = highestHigh()-lowestLow();
	assert(depth >= -0.001); // greater than zero with a little tolerance
	if(depth < 0.0) depth = 0.0;
	return depth;

}

double PeriodValSegment::depthPercent() const // depth from high to low, percent
{
	double high = highestHigh();
	double low = lowestLow();
	return MathHelper::absRelPercentVal(low,high);
}

double PeriodValSegment::pointsAtPercentOfDepthBelowHigh(double percentBelowHigh) const
{
	assert(percentBelowHigh >= 0.0);
	assert(percentBelowHigh <= 100.0);
	double high = highestHigh();
	double depth = depthPoints();

	double depthAtPerc = high - ((percentBelowHigh /100.0) * depth);
	assert(depthAtPerc >= 0.0);

	return depthAtPerc;
}

LinearEquationPtr PeriodValSegment::segmentEquation(const PeriodValueRef &endPointValRef) const
{
	double startPtXVal = this->firstVal().pseudoXVal();
	double endPtXVal = this->lastVal().pseudoXVal();
	assert(endPtXVal > startPtXVal);

	XYCoord startPt(startPtXVal,endPointValRef.referencedVal(this->firstVal()));
	XYCoord endPt(endPtXVal,endPointValRef.referencedVal(this->lastVal()));

	return LinearEquationPtr(new LinearEquation(startPt,endPt));

}

PeriodValSegmentPtr PeriodValSegment::readFromFile(const std::string &fileName)
{
	PeriodValCltnPtr segData = PeriodVal::readFromFile(fileName);
	return PeriodValSegmentPtr(new PeriodValSegment(segData));
}


PeriodValSegment::~PeriodValSegment() {
}

