/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricTrianglePatternMatch.h>
#include <math.h>
#include "PatternMatchVisitor.h"

SymetricTrianglePatternMatch::SymetricTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
        const ChartSegmentPtr &upperTrendLine,
        const ChartSegmentPtr &lowerTrendLine)
    : TrianglePatternMatch(wedgeSegment,upperTrendLine,lowerTrendLine)
{

}

void SymetricTrianglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    TrianglePatternMatch::acceptVisitor(visitor);
	visitor.visitSymetricWedgePatternMatch(*this);
}
