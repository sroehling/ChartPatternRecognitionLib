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

bool SymetricWedgeScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine) const
{

    DoubleRange upperTrendLineSlopeRange(-100.0,-0.10);
    DoubleRange lowerTrendLineSlopeRange(0.10,100.0);

	if(upperTrendLineSlopeRange.valueWithinRange(upperTrendLine->slope()) &&
			lowerTrendLineSlopeRange.valueWithinRange(lowerTrendLine->slope()))
	{
		return WedgeScanner::validTrendLines(upperTrendLine,lowerTrendLine);
	}
	else
	{
		return false;
	}
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

