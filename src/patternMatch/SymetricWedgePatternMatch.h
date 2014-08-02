/*
 * WedgePatternMatch.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef SYMETRICWEDGEPATTERNMATCH_H_
#define SYMETRICWEDGEPATTERNMATCH_H_

#include <PatternMatch.h>
#include "ChartSegment.h"

class SymetricWedgePatternMatch: public PatternMatch {
private:
	ChartSegmentPtr upperTrendLine_;
	ChartSegmentPtr lowerTrendLine_;
public:
	SymetricWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
			const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine);
	virtual void acceptVisitor(PatternMatchVisitor &visitor);
	virtual ~SymetricWedgePatternMatch() {}
};

#endif /* SYMETRICWEDGEPATTERNMATCH_H_ */
