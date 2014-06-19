/*
 * PeriodValSegment.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <PeriodValSegment.h>
#include <iterator>


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

PeriodValSegment::~PeriodValSegment() {
}

