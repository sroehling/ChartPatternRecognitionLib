/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricWedgePatternMatch.h>
#include <math.h>
#include "PatternMatch.h"
#include "PatternMatchVisitor.h"


SymetricWedgePatternMatch::SymetricWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
		const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine)
: PatternMatch(wedgeSegment),
  upperTrendLine_(upperTrendLine),
  lowerTrendLine_(lowerTrendLine)
{

}

void SymetricWedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
	visitor.visitSymetricWedgePatternMatch(*this);
}


PeriodValCltn::iterator SymetricWedgePatternMatch::patternBeginIter() const
{
    // The pattern always beings with the pivot high which starts the first trend line.
    return upperTrendLine_->firstValIter();
}

PeriodValCltn::iterator SymetricWedgePatternMatch::interceptEndIter() const
{
    XYCoord interceptCoord = lowerTrendLine_->segmentEq()->intercept(*(upperTrendLine_->segmentEq()));

    // Intercept must start after initial pivot high occuring at the first value
    // of the upper trend line.
    assert(interceptCoord.x() > (*patternBeginIter()).pseudoXVal());

    // Throughout this library, all the calculations are done on a "fly-weight" to the
    // same underlying PeriodValCltnPtr. Assuming this, the unerlyting PeriodValCltnPtr
    // is obtained from the upperTrendLine_.
    PeriodValCltnPtr perValCltn = upperTrendLine_->perValSegment()->perValCltn();
    unsigned int interceptPerValIndex = floor(interceptCoord.x());
    assert(interceptPerValIndex >= 0);
    if(interceptPerValIndex >= perValCltn->size())
    {
        // Special case: If the intercept occurs after the end of the entire underlying
        // PeriodValSegment, then the iterator should point to the last value in the PeriodValSegment
        // The trend-lines may not intersect by this point, but this is the last value representing
        // the closest point within the actual data where the intersection takes place.
        return perValCltn->end();
    }
    else
    {
        PeriodValCltn::iterator lastValIter = perValCltn->begin();
        std::advance(lastValIter,interceptPerValIndex);
        assert(lastValIter != perValCltn->end());
        assert((*lastValIter).perValIndex() == interceptPerValIndex);
        lastValIter++; // advance one more, so the iterator can function like end() in a conventional STL iterator loop
        return lastValIter;
    }

}
