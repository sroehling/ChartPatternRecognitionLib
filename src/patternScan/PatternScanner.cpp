/*
 * PatternScanner.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include <algorithm>
#include "PatternScanner.h"
#include "ChartSegment.h"
#include "PeriodVal.h"

PatternScanner::PatternScanner(const SegmentConstraintPtr &segmentConstraint,
		const SegmentListConstraintPtr &segmentListConstraint,
		const PatternMatchValidatorPtr &patternMatchValidator)
: segmentConstraint_(segmentConstraint),
  segmentListConstraint_(segmentListConstraint),
  patternMatchValidator_(patternMatchValidator)
{
	minSegmentLength_ = 2;
	maxSegmentLength_ = 200;
}

// Note the splitPeriodVals method on a list is 1 based: in other
// words, for beforeSplitPos to have 3 elements
// you'd call splitPeriodVals with a splitPos=3
// TODO - This method should be setup as a utility method in the PeriodVal class.
void PatternScanner::splitPeriodVals(const PeriodValCltn &srcCltn,
			unsigned int splitPos, PeriodValCltn &beforeSplitPos,
			PeriodValCltn &afterSplitPos)
{
	assert(splitPos <= srcCltn.size());

	beforeSplitPos.clear();
	afterSplitPos.clear();

	unsigned int currPos = 0;
	for(PeriodValCltn::const_iterator i = srcCltn.begin(); i != srcCltn.end(); i++)
	{
		if(currPos < splitPos)
		{
			beforeSplitPos.push_back(*i);
		}
		else
		{
			afterSplitPos.push_back(*i);
		}
		currPos++;
	}
}

// scanPatternMatches is the recursive function for building up potential
// pattern matches.
PatternMatchListPtr PatternScanner::scanPatternMatches(
		const ChartSegmentList &leadingSegments,
		const PeriodValCltn &remainingVals)
{
	// matchingPatterns remains empty, unless a pattern match is
	// completed at this depth of recursion.
	PatternMatchListPtr matchingPatterns = PatternMatchListPtr(new PatternMatchList());

	if(remainingVals.size() < minSegmentLength_)
	{
		// There's not enough values remaining to continue
		// matching, so return an empty list of matches.
		return matchingPatterns;
	}

	unsigned int minRemSegmentLen = minSegmentLength_;
	unsigned int maxRemSegmentLen = std::max((unsigned int)remainingVals.size(),
					maxSegmentLength_);

	for(unsigned int splitPos = minRemSegmentLen; splitPos < maxRemSegmentLen; splitPos++)
	{
		assert(splitPos < remainingVals.size());

		PeriodValCltn leadingVals, trailingVals;
		splitPeriodVals(remainingVals,splitPos,leadingVals,trailingVals);

		// If the segment being created is not the first, it needs
		// to include the last value from the previous segment. This
		// ensures the line traced out by the segments is uninterrupted.
		// In other words the end point for the previous segment becomes
		// the starting point for the next segment.
		if(leadingSegments.size() > 0)
		{
			PeriodVal lastValFromPrevSeg = leadingSegments.back()->lastPeriodVal();
			leadingVals.push_front(lastValFromPrevSeg);
		}

		ChartSegmentPtr leadingSeg(new ChartSegment(leadingVals));
		ChartSegmentList appendedSegments(leadingSegments);
		appendedSegments.push_back(leadingSeg);

		// Both the individual segment and the appended list of segments must be valid
		// for the matching/scanning to continue.Where these contraints
		// are used to progressively test the pattern is well-formed,
		// there's only a need to continue (recursively) scanning if
		// the appendedSegments are valid for all constraints.
		if(segmentConstraint_->validSegment(*leadingSeg) &&
				segmentListConstraint_->validSegments(appendedSegments))
		{
			PatternMatchPtr candidateMatch(new PatternMatch(appendedSegments));
			if(patternMatchValidator_->validPattern(*candidateMatch))
			{
				matchingPatterns->push_back(candidateMatch);
			}

			PatternMatchListPtr subMatches = scanPatternMatches(appendedSegments,
					trailingVals);

			// Append the sub-matches to the list returned from this
			// depth of the recursion.
			matchingPatterns->insert(matchingPatterns->end(),
						subMatches->begin(),subMatches->end());
		}


	} // for each candidate segment starting at the current point

	// Return the list of pattern matches both at this level and
	// below this level in the recursion (further along in the price data).
	return matchingPatterns;
}

PatternMatchListPtr PatternScanner::scanPatternMatches(const PeriodValCltn &chartVals)
{
	ChartSegmentList emptySegmentList;

	PatternMatchListPtr matchingPatterns = scanPatternMatches(emptySegmentList,chartVals);

	// An individual PatternScanner can return
	// multiple valid pattern matches.
	//
	// An individual PatternScanner can be used to match different sub-patterns,
	// then the sub-patterns can be strung together to form a larger pattern
	// (e.g., match the base of cup with handle, then
	// match the handle).Therefore, the end-point of these patterm matches
	// can serve as the starting point to match other sub-patterns.
	return matchingPatterns;

}


PatternScanner::~PatternScanner() {
	// TODO Auto-generated destructor stub
}

