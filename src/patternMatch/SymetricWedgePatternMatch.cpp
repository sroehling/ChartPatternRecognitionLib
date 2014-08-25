/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricWedgePatternMatch.h>
#include <math.h>
#include "PatternMatchVisitor.h"

SymetricWedgePatternMatch::SymetricWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : WedgePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void SymetricWedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    WedgePatternMatch::acceptVisitor(visitor);
	visitor.visitSymetricWedgePatternMatch(*this);
}
