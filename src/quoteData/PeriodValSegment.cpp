/*
 * PeriodValSegment.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <PeriodValSegment.h>
#include <iterator>
#include "MathHelper.h"


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

const PeriodVal &PeriodValSegment::lastVal() const
{
	assert(numVals() > 0);
	PeriodValCltn::iterator lastValIter = segEnd_;
	lastValIter--;
	return (*lastValIter);
}

double PeriodValSegment::highestHigh() const
{
	double maxVal = MathHelper::minDouble();
	for(PeriodValCltn::const_iterator perValIter = segBegin();
			perValIter != segEnd(); perValIter++)
	{
		if((*perValIter).high() > maxVal)
		{
			maxVal = (*perValIter).high();
		}
	}
	assert(maxVal >= 0.0);
	return maxVal;
}

double PeriodValSegment::lowestLow() const
{
	double minVal = MathHelper::maxDouble();
	for(PeriodValCltn::const_iterator perValIter = segBegin();
			perValIter != segEnd(); perValIter++)
	{
		if((*perValIter).low() < minVal)
		{
			minVal = (*perValIter).low();
		}
	}
	assert(minVal >= 0.0);
	return minVal;
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



PeriodValSegment::~PeriodValSegment() {
}

