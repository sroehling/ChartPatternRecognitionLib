/*
 * WedgePatternMatch.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef WEDGEPATTERNMATCH_H_
#define WEDGEPATTERNMATCH_H_

#include <PatternMatch.h>
#include "ChartSegment.h"

class WedgePatternMatch: public PatternMatch {
private:
	ChartSegmentPtr upperTrendLine_;
	ChartSegmentPtr lowerTrendLine_;
public:
	WedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
			const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine);
	virtual void acceptVisitor(PatternMatchVisitor &visitor);
	virtual ~WedgePatternMatch() {}
};

#endif /* WEDGEPATTERNMATCH_H_ */
