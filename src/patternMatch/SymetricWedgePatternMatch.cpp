/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricWedgePatternMatch.h>
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
