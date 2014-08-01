/*
 * WedgePatternMatch.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <WedgePatternMatch.h>
#include "PatternMatch.h"
#include "PatternMatchVisitor.h"

WedgePatternMatch::WedgePatternMatch(const ChartSegmentList &segments,
		const ChartSegmentPtr &upperTrendLine,
		ChartSegmentPtr &lowerTrendLine)
: PatternMatch(segments),
  upperTrendLine_(upperTrendLine),
  lowerTrendLine_(lowerTrendLine)
{

}

void WedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
	visitor.visitWedgePatternMatch(*this);
}
