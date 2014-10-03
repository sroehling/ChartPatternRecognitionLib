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

PatternMatch::PatternMatch(const ChartSegmentPtr &singleSegment)
{
	segments_.push_back(singleSegment);
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

const PeriodVal &PatternMatch::secondToLastValue() const
{
    PeriodValCltn::iterator lastValIter = matchSegment_->lastValIter();
    assert(lastValIter != matchSegment_->segBegin());
    lastValIter--;
    return (*lastValIter);
}

const boost::posix_time::ptime &PatternMatch::startTime() const
{
	return this->firstValue().periodTime();
}

const boost::posix_time::ptime &PatternMatch::endTime() const
{
	return this->lastValue().periodTime();
}

const PeriodValCltn::iterator &PatternMatch::beginMatchIter() const
{
    return matchSegment_->segBegin();
}

const PeriodValCltn::iterator &PatternMatch::endMatchIter() const
{
    return matchSegment_->segEnd();
}

unsigned int PatternMatch::numSegments() const
{
	return segments_.size();
}


PeriodValSegmentPtr PatternMatch::trailingValsWithLastVal() const
{
	return segments_.back()->perValSegment()->trailingValsWithLastVal();
}

PeriodValSegmentPtr PatternMatch::trailingValsWithLastVal(unsigned int maxVals) const
{
    return segments_.back()->perValSegment()->trailingValsWithLastVal(maxVals);
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
        for(unsigned int segIndex = 1; segIndex < segments.size()-1; segIndex++)
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

void PatternMatch::initBreakoutAboveFirstHighBreakoutInfo(const PatternMatchPtr &match)
{
    PeriodVal firstVal = match->firstValue();
    double breakoutThreshold = firstVal.high();

    PeriodVal lastVal = match->lastValue();
    PeriodVal preBreakoutVal = match->secondToLastValue();

    double breakoutPseudoX = lastVal.pseudoXVal();

    // If the pattern includes a breakout above the middle of the center V, include the breakout information
    // so it can be plotted. Otherwise, the pattern match is likely a partial match which can be plotted, but doesn't
    // include a breakout.
    //
    // TODO - Somehow support plotting of the partial breakout, where the threshold is known but the breakout hasn't
    // occured yet.
    if((preBreakoutVal.close() <= breakoutThreshold) &&
            (lastVal.close() > breakoutThreshold))
    {
        this->breakoutInfo = PatternMatchBreakoutInfoPtr(new PatternMatchBreakoutInfo(breakoutPseudoX,breakoutThreshold));
    }

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

const boost::posix_time::ptime PatternMatch::lowestLowTime() const
{
	return lowestLowVal().periodTime();
}

const PeriodValCltn::iterator &PatternMatch::lowestLowIter() const
{
	return matchSegment_->lowestLowIter();
}



double PatternMatch::highestHigh() const
{
	return matchSegment_->highestHigh();
}

PeriodVal PatternMatch::highestHighVal() const
{
	return matchSegment_->highestHighVal();
}

const boost::posix_time::ptime PatternMatch::highestHighTime() const
{
	return highestHighVal().periodTime();
}


const PeriodValCltn::iterator &PatternMatch::highestHighIter() const
{
	return matchSegment_->highestHighIter();
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


bool PatternMatch::isConfirmedMatch() const
{
    if((this->breakdownInfo) || (this->breakoutInfo))
    {
        return true;
    }
    else
    {
        return false;
    }
}

double PatternMatch::confirmationPrice() const
{
    assert(isConfirmedMatch()); // undefined if not a confirmed match

    if(this->breakdownInfo)
    {
        return this->breakdownInfo->breakoutPrice();
    }
    else if(this->breakoutInfo)
    {
        return this->breakoutInfo->breakoutPrice();
    }
    return -1.0; // undefined, should not get here

}

PatternMatch::~PatternMatch() {
}

