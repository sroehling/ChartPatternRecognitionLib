/*
 * PeriodValSegment.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <PeriodValSegment.h>
#include <iterator>
#include "MathHelper.h"


void PeriodValSegment::initHighestHighValIter()
{
	double maxVal = MathHelper::minDouble();
	highestHighValIter_ = segBegin();
	for(PeriodValCltn::iterator perValIter = segBegin();
			perValIter != segEnd(); perValIter++)
	{
		if((*perValIter).high() > maxVal)
		{
			maxVal = (*perValIter).high();
			highestHighValIter_ = perValIter;
		}
	}
}

void PeriodValSegment::initLowestLowValIter()
{
	double minVal = MathHelper::maxDouble();
	lowestLowValIter_ = segBegin();
	for(PeriodValCltn::iterator perValIter = segBegin();
			perValIter != segEnd(); perValIter++)
	{
		if((*perValIter).low() < minVal)
		{
			minVal = (*perValIter).low();
			lowestLowValIter_ = perValIter;
		}
	}
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

unsigned int PeriodValSegment::numVals() const
{
	return endPos_- startPos_;
}

const PeriodVal &PeriodValSegment::firstVal() const
{
	assert(numVals() > 0);
	return (*segBegin_);

}

const PeriodValCltn::iterator &PeriodValSegment::lastValIter() const
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

