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

class PatternMatch {
private:
	ChartSegmentList segments_;
	bool segmentsConnected(const ChartSegmentList &segments) const;
public:
	PatternMatch(const ChartSegmentList &segments);

	const PeriodVal &firstValue() const;
	const PeriodVal &lastValue() const;
	unsigned int numSegements() const;

	virtual ~PatternMatch();
};

typedef boost::shared_ptr<PatternMatch> PatternMatchPtr;
typedef std::vector<PatternMatchPtr> PatternMatchList;
typedef boost::shared_ptr<PatternMatchList> PatternMatchListPtr;

#endif /* PATTERNMATCH_H_ */
