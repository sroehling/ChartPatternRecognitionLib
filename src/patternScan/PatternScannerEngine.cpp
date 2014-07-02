/*
 * PatternScanner.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include <algorithm>
#include "PatternScannerEngine.h"
#include "ChartSegment.h"
#include "PeriodVal.h"

PatternScannerEngine::PatternScannerEngine(const SegmentConstraintPtr &segmentConstraint,
		const SegmentListConstraintPtr &segmentListConstraint,
		const PatternMatchValidatorPtr &patternMatchValidator)
: segmentConstraint_(segmentConstraint),
  segmentListConstraint_(segmentListConstraint),
  patternMatchValidator_(patternMatchValidator)
{
	minSegmentLength_ = 3;
	maxSegmentLength_ = 200;
}


// scanPatternMatches is the recursive function for building up potential
// pattern matches.
PatternMatchListPtr PatternScannerEngine::scanPatternMatches(
		const ChartSegmentList &leadingSegments,
		const PeriodValSegmentPtr &remainingVals) const
{
	// matchingPatterns remains empty, unless a pattern match is
	// completed at this depth of recursion.
	PatternMatchListPtr matchingPatterns = PatternMatchListPtr(new PatternMatchList());

	unsigned int minRemSegmentLen = minSegmentLength_;
	if(leadingSegments.size()>0)
	{
		// If there's at least one leading segment, then minRemSegmentLen
		// used to iterate and split the remaining period values can
		// be decreased by one, since the leading segment will include
		// the last value from the previous segment.
		minRemSegmentLen--;
	}
	unsigned int maxRemSegmentLen = std::min((unsigned int)remainingVals->numVals(),
					maxSegmentLength_);

	if(remainingVals->numVals() < minRemSegmentLen)
	{
		// There's not enough values remaining to continue
		// matching, so return an empty list of matches.
		return matchingPatterns;
	}

	// The for loop logic includes a '<=' since a splitPos for mxRemSegmentLen
	// will return all the remaining values.
	for(unsigned int splitPos = minRemSegmentLen; splitPos <= maxRemSegmentLen; splitPos++)
	{
		PeriodValSegmentPair segmentSplit = remainingVals->split(splitPos);
		PeriodValSegmentPtr leadingVals = segmentSplit.first;
		PeriodValSegmentPtr trailingVals = segmentSplit.second;

		// If the segment being created is not the first, it needs
		// to include the last value from the previous segment. This
		// ensures the line traced out by the segments is uninterrupted.
		// In other words the end point for the previous segment becomes
		// the starting point for the next segment.
		if(leadingSegments.size() > 0)
		{
			leadingVals = leadingVals->moveSegBeginToPrev();
		}

		ChartSegmentPtr leadingSeg(new ChartSegment(leadingVals));
		ChartSegmentList appendedSegments(leadingSegments);
		appendedSegments.push_back(leadingSeg);

		// Both the individual segment and the appended list of segments must be valid
		// for the matching/scanning to continue.Where these constraints
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

PatternMatchListPtr PatternScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	ChartSegmentList emptySegmentList;

	PatternMatchListPtr matchingPatterns = scanPatternMatches(emptySegmentList,chartVals);

	// An individual PatternScanner can return
	// multiple valid pattern matches.
	//
	// An individual PatternScanner can be used to match different sub-patterns,
	// then the sub-patterns can be strung together to form a larger pattern
	// (e.g., match the base of cup with handle, then
	// match the handle).Therefore, the end-point of these pattern matches
	// can serve as the starting point to match other sub-patterns.
	return matchingPatterns;

}


PatternScannerEngine::~PatternScannerEngine() {
	// TODO Auto-generated destructor stub
}

