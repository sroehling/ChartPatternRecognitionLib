/*
 * SymetricWedgeScanner.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricWedgeScanner.h>
#include "TrendLineScanner.h"
#include "ChartSegment.h"
#include "SymetricWedgePatternMatch.h"

SymetricWedgeScanner::SymetricWedgeScanner()
    : WedgeScanner(WedgeScanner::DOWNTREND_SLOPE_RANGE,WedgeScanner::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr SymetricWedgeScanner::findPatternMatch(const PeriodValSegmentPtr &chartVals,
		const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine,
		const PeriodValCltn::iterator &currPerValIter) const
{
	if (upperTrendLineBreakout(chartVals,upperTrendLine,currPerValIter))
	{
		ChartSegmentPtr wedgeSeg = createWedgeSegment(chartVals,upperTrendLine,currPerValIter);
		return PatternMatchPtr(new SymetricWedgePatternMatch(wedgeSeg,upperTrendLine,lowerTrendLine));
	}

	return PatternMatchPtr(); // NULL (smart) pointer

}

