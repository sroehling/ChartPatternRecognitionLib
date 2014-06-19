/*
 * PatternMatch.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PatternMatch.h"

PatternMatch::PatternMatch(const ChartSegmentList &segments)
: segments_(segments)
{
	assert(segments.size() > 0);
	assert(segmentsConnected(segments_));
}


const PeriodVal &PatternMatch::firstValue() const
{
	return segments_.front()->firstPeriodVal();
}

const PeriodVal &PatternMatch::lastValue() const
{
	return segments_.back()->lastPeriodVal();
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
		for(int segIndex = 1; segIndex < segments.size()-1; segIndex++)
		{
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
	// TODO Auto-generated destructor stub
}

