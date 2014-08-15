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

    ChartSegmentPtr upperTrendLine() const { return upperTrendLine_; }
    ChartSegmentPtr lowerTrendLine() const { return lowerTrendLine_; }

    // Iterators defining the beginning and end of the trend lines
    // comprising the pattern. These methods are useful for iterating
    // through the period values and drawing the lower and upper
    // trend lines.
    PeriodValCltn::iterator patternBeginIter() const;
    PeriodValCltn::iterator interceptEndIter() const;

	virtual ~SymetricWedgePatternMatch() {}

    virtual std::string matchType() const { return "Symetric Wedge"; }

};

#endif /* SYMETRICWEDGEPATTERNMATCH_H_ */
