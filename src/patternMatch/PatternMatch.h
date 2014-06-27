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
public:
	PatternMatch(const ChartSegmentList &segments);

	const PeriodVal &firstValue() const;
	const PeriodVal &lastValue() const;
	unsigned int numSegments() const;
	unsigned int numPeriods() const;
	const ChartSegmentList &segments() const;

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
