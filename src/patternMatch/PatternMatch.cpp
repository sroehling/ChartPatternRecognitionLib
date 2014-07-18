/*
 * PatternMatch.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PatternMatch.h"
#include "MathHelper.h"

void PatternMatch::initMatchSegment()
{
	PeriodValCltn::iterator matchSegBegin = segments().front()->perValSegment()->segBegin();
	PeriodValCltn::iterator matchSegEnd = segments().back()->perValSegment()->segEnd();
	PeriodValCltnPtr matchPerValCltn = segments().front()->perValSegment()->perValCltn();
	PeriodValSegmentPtr combinedMatchSegment(new PeriodValSegment(matchPerValCltn,matchSegBegin,matchSegEnd));
	matchSegment_ = combinedMatchSegment;
}


PatternMatch::PatternMatch(const ChartSegmentList &segments)
: segments_(segments)
{
	assert(segments.size() > 0);
	assert(segmentsConnected(segments_));
	initMatchSegment();
}

const PeriodVal &PatternMatch::firstValue() const
{
	return segments_.front()->firstPeriodVal();
}

const PeriodVal &PatternMatch::lastValue() const
{
	return segments_.back()->lastPeriodVal();
}

const boost::posix_time::ptime &PatternMatch::startTime() const
{
	return this->firstValue().periodTime();
}

const boost::posix_time::ptime &PatternMatch::endTime() const
{
	return this->lastValue().periodTime();
}


unsigned int PatternMatch::numSegments() const
{
	return segments_.size();
}


PeriodValSegmentPtr PatternMatch::trailingValsWithLastVal() const
{
	return segments_.back()->perValSegment()->trailingValsWithLastVal();
}

PatternMatchPtr PatternMatch::appendMatch(const PatternMatch &matchToAppend) const
{
	ChartSegmentList appendedSegments = segments_;
	appendedSegments.insert(appendedSegments.end(),matchToAppend.segments().begin(),
			matchToAppend.segments().end());
	return PatternMatchPtr(new PatternMatch(appendedSegments));
}

PatternMatchListPtr PatternMatch::appendMatchList(const PatternMatchList &appendList) const
{
	PatternMatchListPtr appendedList(new PatternMatchList());
	for(PatternMatchList::const_iterator appendIter = appendList.begin();
			appendIter != appendList.end(); appendIter++)
	{
		appendedList->push_back(appendMatch(**appendIter));
	}
	return appendedList;
}



// This is a validation method to ensure when chart segments are "stitched together"
// for purposes of forming patterns, the PeriodVal at the end of the preceding segment
// is the same as the first PeriodVal in the next segment.
bool PatternMatch::segmentsConnected(const ChartSegmentList &segments) const
{
	if(segments.size() == 1)
	{
		return true;
	}
	else
	{
		PeriodValCltn::iterator firstSegBeginIter =
					segments.front()->perValSegment()->perValCltn()->begin();
		for(int segIndex = 1; segIndex < segments.size()-1; segIndex++)
		{
			// Segments must be point to the same underlying PeriodValCltn. This is
			// necessary, since segments are implemented as flyweights for PeriodValCltn,
			// and some of the calculations in this class are dependent on the
			// segments referencing the same PeriodValCltn.
			PeriodValCltn::iterator currSegBeginIter =
					segments[segIndex]->perValSegment()->perValCltn()->begin();
			assert(currSegBeginIter==firstSegBeginIter);

			int prevSegIndex = segIndex - 1;
			if(segments[prevSegIndex]->lastPeriodVal().periodTime() !=
					segments[segIndex]->firstPeriodVal().periodTime())
			{
				return false;
			}
		}
	}
	return true;
}

const ChartSegmentList &PatternMatch::segments() const
{
	return segments_;
}


unsigned int PatternMatch::numPeriods() const
{
	return matchSegment_->numVals();
}

double PatternMatch::lowestLow() const
{
	return matchSegment_->lowestLow();
}

PeriodVal PatternMatch::lowestLowVal() const
{
	return matchSegment_->lowestLowVal();
}

double PatternMatch::highestHigh() const
{
	return matchSegment_->highestHigh();
}

PeriodVal PatternMatch::highestHighVal() const
{
	return matchSegment_->highestHighVal();
}


double PatternMatch::depthPoints() const
{
	return matchSegment_->depthPoints();
}

double PatternMatch::depthPercent() const
{
	return matchSegment_->depthPercent();
}

double PatternMatch::pointsAtPercentOfDepthBelowHigh(double percentFromHigh) const
{
	return matchSegment_->pointsAtPercentOfDepthBelowHigh(percentFromHigh);
}


std::ostream& operator<<(std::ostream& os, const PatternMatch& patternMatch)
{
	os << "PatternMatch(num segments=" << patternMatch.numSegments()
			<< ", segments = (";
	bool firstSeg = true;
	for(ChartSegmentList::const_iterator segIter = patternMatch.segments_.begin();
			segIter != patternMatch.segments_.end();segIter++)
	{
		if(!firstSeg)
		{
			os << ", ";
		}

		// A ChartSegmentList is a list of smart pointers to chart segments,
		// so we need to de-reference both the iterator and (smart) pointer.
		os << **segIter;
		firstSeg = false;
	}
	os		<< "))";
	return os;
}


PatternMatch::~PatternMatch() {
}

