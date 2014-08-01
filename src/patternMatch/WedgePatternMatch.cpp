/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <WedgePatternMatch.h>
#include "PatternMatch.h"
#include "PatternMatchVisitor.h"


WedgePatternMatch::WedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
		const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine)
: PatternMatch(wedgeSegment),
  upperTrendLine_(upperTrendLine),
  lowerTrendLine_(lowerTrendLine)
{

}

void WedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
	visitor.visitWedgePatternMatch(*this);
}
