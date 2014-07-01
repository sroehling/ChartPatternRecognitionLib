/*
 * PatternMatch.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCH_H_
#define PATTERNMATCH_H_

#include <boost/shared_ptr.hpp>
#include "ChartSegment.h"

class PatternMatch;
typedef boost::shared_ptr<PatternMatch> PatternMatchPtr;
typedef std::vector<PatternMatchPtr> PatternMatchList;
typedef boost::shared_ptr<PatternMatchList> PatternMatchListPtr;


class PatternMatch {
private:
	ChartSegmentList segments_;
	bool segmentsConnected(const ChartSegmentList &segments) const;

	// Construct a combined segment from the start to the end of the
	// pattern match. This is useful for performing internal calculations
	// on pattern depth, lowest lows, etc.
	PeriodValSegmentPtr matchSegment() const;
public:
	PatternMatch(const ChartSegmentList &segments);

	const PeriodVal &firstValue() const;
	const PeriodVal &lastValue() const;
	unsigned int numSegments() const;
	unsigned int numPeriods() const;
	const ChartSegmentList &segments() const;

	double highestHigh() const; // Highest high across all segments
	double lowestLow() const; // Lowest low across all segments
	double depthPoints() const; // depth from highest high to lowest low
	double depthPercent() const; // depth from highest high to lowest low

	double pointsAtPercentOfDepthBelowHigh(double percentFromHigh) const;

	// Return a segment containing all the PeriodVal's after this
	// pattern match, and including the last value. This is needed
	// for matching sub-patterns, then putting/stitching together
	// the sub-patterns into a combined pattern.
	PeriodValSegmentPtr trailingValsWithLastVal() const;

	PatternMatchPtr appendMatch(const PatternMatch &matchToAppend) const;
	PatternMatchListPtr appendMatchList(const PatternMatchList &appendList) const;

	friend std::ostream& operator<<(std::ostream& os, const PatternMatch& patternMatch);


	virtual ~PatternMatch();
};


#endif /* PATTERNMATCH_H_ */
