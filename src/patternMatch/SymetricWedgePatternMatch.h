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

#include "WedgePatternMatch.h"

class SymetricWedgePatternMatch: public WedgePatternMatch {
public:
    SymetricWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

	virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Symetric Wedge"; }

};

#endif /* SYMETRICWEDGEPATTERNMATCH_H_ */
