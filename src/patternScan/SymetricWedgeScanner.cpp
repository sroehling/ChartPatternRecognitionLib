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

PatternMatchPtr SymetricWedgeScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    if (wedgeMatchValidationInfo.upperTrendLineBreakout())
	{
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        return PatternMatchPtr(new SymetricWedgePatternMatch(wedgeSeg,
              wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
	}

	return PatternMatchPtr(); // NULL (smart) pointer

}

