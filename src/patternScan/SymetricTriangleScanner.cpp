/*
 * SymetricWedgeScanner.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <SymetricTriangleScanner.h>
#include "TrendLineScanner.h"
#include "ChartSegment.h"
#include "SymetricTrianglePatternMatch.h"

SymetricTriangleScanner::SymetricTriangleScanner()
    : TriangleScanner(WedgeScannerEngine::DOWNTREND_SLOPE_RANGE,WedgeScannerEngine::UPTREND_SLOPE_RANGE)
{
}

PatternMatchPtr SymetricTriangleScanner::findPatternMatch(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PatternMatchBreakoutInfoPtr breakoutInfo = wedgeMatchValidationInfo.upperTrendLineBreakout();
    if (breakoutInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch = PatternMatchPtr(new SymetricTrianglePatternMatch(wedgeSeg,
              wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakoutInfo = breakoutInfo;
        return patternMatch;
	}

    PatternMatchBreakoutInfoPtr breakdownInfo = wedgeMatchValidationInfo.lowerTrendLineBreakdown();
    if (breakdownInfo)
    {
        ChartSegmentPtr wedgeSeg = wedgeMatchValidationInfo.createWedgeSegment();
        PatternMatchPtr patternMatch = PatternMatchPtr(new SymetricTrianglePatternMatch(wedgeSeg,
              wedgeMatchValidationInfo.upperTrendLine(),wedgeMatchValidationInfo.lowerTrendLine()));
        patternMatch->breakdownInfo = breakdownInfo;
        return patternMatch;
    }


	return PatternMatchPtr(); // NULL (smart) pointer

}

