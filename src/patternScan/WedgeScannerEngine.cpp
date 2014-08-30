/*
 * WedgeScannerEngine.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
#include <WedgeScannerEngine.h>
#include "PeriodValSegment.h"
#include "PatternMatchFilter.h"
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "DoubleRange.h"
#include "PeriodValueRef.h"
#include "PivotLowScanner.h"
#include "PivotHighScanner.h"
#include "Math.h"
#include "UnsignedIntRange.h"
#include "WedgeMatchValidationInfo.h"

const double WedgeScannerEngine::PERC_CLOSING_VALS_INSIDE_TRENDLINES_THRESHOLD = 0.20;
const double WedgeScannerEngine::RATIO_ABOVE_VS_BELOW_TRENDLINE_MIDPOINT_THRESHOLD = 3.0;
const double WedgeScannerEngine::MAX_DISTANCE_OUTSIDE_TRENDLINE_PERC_OF_CURR_DEPTH = 0.25;

WedgeScannerEngine::WedgeScannerEngine() {
}

bool WedgeScannerEngine::pivotsInterleaved(const ChartSegmentPtr &upperTrendLine,
                       const ChartSegmentPtr &lowerTrendLine) const
{
    // The first and last value of the upper and lower trend lines are the pivot highs and lows used to
    // define the trend lines. With this in mind, we don't want to consider a pattern valid if
    // there is not a pivot low before the second pivot high; in other words, the pattern is not
    // considered valid (i.e., well-balanced) if both the pivot lows occur after both the pivot highs.
    if(lowerTrendLine->firstPeriodVal().periodTime() < upperTrendLine->lastPeriodVal().periodTime())
    {
        return true;
    }

    return false;

}


double WedgeScannerEngine::percClosingValsOutsideTrendLines(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PeriodValCltn::iterator patternEndIter = wedgeMatchValidationInfo.patternEndIter();

    double valsOutSideTrendLines = 0.0;
    double totalVals = 0.0;
    for(PeriodValCltn::iterator perValIter =  wedgeMatchValidationInfo.patternBeginIter();
        perValIter != patternEndIter; perValIter++)
    {
        if (wedgeMatchValidationInfo.upperTrendLine()->segmentEq()->aboveLine((*perValIter).closeCoord()) ||
                wedgeMatchValidationInfo.lowerTrendLine()->segmentEq()->belowLine((*perValIter).closeCoord()))
        {
            valsOutSideTrendLines += 1.0;
        }
        totalVals ++;
    }

    assert(totalVals > 0.0);
    double percOutsideTrendLines = valsOutSideTrendLines/totalVals;
    return percOutsideTrendLines;
}

bool WedgeScannerEngine::allClosingValsWithinThresholdOutsideTrendLines(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    // The interation needs to end *before* currPerValIter, since
    // wedgeMatchValidationInfo.currPerValIter() could include a break-out/break-down
    // which invalidates this rule.
    PeriodValCltn::iterator patternEndIter = wedgeMatchValidationInfo.currPerValIter();


    for(PeriodValCltn::iterator perValIter =  wedgeMatchValidationInfo.patternBeginIter();
        perValIter != patternEndIter; perValIter++)
    {
        double currXVal = (*perValIter).pseudoXVal();
        double upperYVal = wedgeMatchValidationInfo.upperTrendLine()->segmentEq()->yVal(currXVal);
        double lowerYVal = wedgeMatchValidationInfo.lowerTrendLine()->segmentEq()->yVal(currXVal);
        assert(upperYVal >= lowerYVal);
        double currDepth = upperYVal-lowerYVal;
        double currDistanceThresholdOutsideTrendLine = MAX_DISTANCE_OUTSIDE_TRENDLINE_PERC_OF_CURR_DEPTH * currDepth;
        double currClose = (*perValIter).close();

       if (wedgeMatchValidationInfo.upperTrendLine()->segmentEq()->aboveLine((*perValIter).closeCoord()))
       {
            double distanceAbove = currClose-upperYVal;
            assert(distanceAbove >= 0.0);
            if(distanceAbove > currDistanceThresholdOutsideTrendLine)
            {
                return false;
            }
       }
       else if (wedgeMatchValidationInfo.lowerTrendLine()->segmentEq()->belowLine((*perValIter).closeCoord()))
       {
            double distanceBelow = lowerYVal - currClose;
            assert(distanceBelow >= 0.0);
            if(distanceBelow > currDistanceThresholdOutsideTrendLine)
            {
                return false;
            }
       }
    }

    return true;
}




bool WedgeScannerEngine::percClosingValsOutsideTrendLinesWithinThreshold(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    if(percClosingValsOutsideTrendLines(wedgeMatchValidationInfo)
            > PERC_CLOSING_VALS_INSIDE_TRENDLINES_THRESHOLD)
    {
        return false;
    }

    return true;

}

double WedgeScannerEngine::ratioClosingValsAboveVsBelowMidpoint(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    PeriodValCltn::iterator patternEndIter = wedgeMatchValidationInfo.patternEndIter();

    // Start both the values with a small number, so we never get a divide by zero error.
    double totalAboveMidpoint = 0.000001;
    double totalBelowMidpoint = 0.000001;
    for(PeriodValCltn::iterator perValIter =  wedgeMatchValidationInfo.patternBeginIter();
        perValIter != patternEndIter; perValIter++)
    {

        double currXVal = (*perValIter).pseudoXVal();
        double upperYVal = wedgeMatchValidationInfo.upperTrendLine()->segmentEq()->yVal(currXVal);
        double lowerYVal = wedgeMatchValidationInfo.lowerTrendLine()->segmentEq()->yVal(currXVal);
        assert(upperYVal >= lowerYVal);
        double midpointYVal = (upperYVal + lowerYVal) / 2.0;

        if((*perValIter).close() >= midpointYVal)
        {
            totalAboveMidpoint += 1.0;
        }
        else
        {
            totalBelowMidpoint += 1.0;
        }

     }

    double ratioAboveVsBelow = totalAboveMidpoint/totalBelowMidpoint;

    return ratioAboveVsBelow;
}

bool WedgeScannerEngine::ratioAboveVsBelowMidpointWithinThreshold(
        const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    double ratioAboveBelow = ratioClosingValsAboveVsBelowMidpoint(wedgeMatchValidationInfo);
    if(ratioAboveBelow > RATIO_ABOVE_VS_BELOW_TRENDLINE_MIDPOINT_THRESHOLD)
    {
        return false;
    }
    else if(ratioAboveBelow < (1.0/RATIO_ABOVE_VS_BELOW_TRENDLINE_MIDPOINT_THRESHOLD))
    {
        return false;
    }
    return true;
}


bool WedgeScannerEngine::validWedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const
{
    if(!percClosingValsOutsideTrendLinesWithinThreshold(wedgeMatchValidationInfo))
    {
        return false;
    }

    if(!ratioAboveVsBelowMidpointWithinThreshold(wedgeMatchValidationInfo))
    {
        return false;
    }

    if(!allClosingValsWithinThresholdOutsideTrendLines(wedgeMatchValidationInfo))
    {
        return false;
    }

    return true;
}



PatternMatchListPtr WedgeScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr wedgeMatches(new PatternMatchList());

	// Unlike the PatternScannerEngine, this scanner will return pattern matches starting
	// anywhere from the beginning of chartVals to the end.

	// The scanning starts by getting a list of pivot highs and lows within chartVals.
	// These pivots serve as anchor points for drawing candidate trend lines for the
	// pattern matches.
	PatternMatchListPtr pivotHighs = PivotHighScanner().scanPatternMatches(chartVals);
	PatternMatchListPtr pivotLows = PivotLowScanner().scanPatternMatches(chartVals);

	// Each of the pivot highs serves as a potential starting point for the pattern match.
	for(PatternMatchList::iterator startMatchPivotHighIter = pivotHighs->begin();
			startMatchPivotHighIter != pivotHighs->end(); startMatchPivotHighIter++)
	{

		PeriodValCltn::iterator firstPivotHighIter = (*startMatchPivotHighIter)->highestHighIter();

		PatternMatchList::iterator remainingPivotsBegin = startMatchPivotHighIter;
		remainingPivotsBegin++;
		// The remaining pivot highs are candidates for drawing trend lines to form the pattern.
		for(PatternMatchList::iterator trendLineEndPointPivotHighIter = remainingPivotsBegin;
				trendLineEndPointPivotHighIter != pivotHighs->end(); trendLineEndPointPivotHighIter++)
		{
			PeriodValCltn::iterator secondPivotHighIter = (*trendLineEndPointPivotHighIter)->highestHighIter();
			// Draw the trend line from (*startMatchPivotHighIter) to (*trendLineEndPointPivotHighIter).
			// This trend line is only valid for purposes of continued matching the wedge pattern if it has
			// a negative slope.

			// Find the first pivot low which starts after the starting pivot high. This
			// first pivot low serves as the candidate starting point to draw the corresponding
			// lower trend line.
			PatternMatchList::iterator pivotLowBeginAfterPivotHighStart =
					patternMatchFilter::findFirstPatternMatch(pivotLows,
							LowestLowAfterTime((*startMatchPivotHighIter)->highestHighTime()));
			if(pivotLowBeginAfterPivotHighStart != pivotLows->end())
			{
				for(PatternMatchList::iterator startPivotLowIter = pivotLowBeginAfterPivotHighStart;
						startPivotLowIter != pivotLows->end(); startPivotLowIter++)
				{
					PeriodValCltn::iterator firstPivotLowIter = (*startPivotLowIter)->lowestLowIter();

					// TODO - Test if depth from the pivot high to pivot low satisfies the depth criterion.
					PatternMatchList::iterator remainingPivotLowsBegin = startPivotLowIter;
					remainingPivotLowsBegin++;
					for(PatternMatchList::iterator trendLineEndPointPivotLowIter = remainingPivotLowsBegin;
							trendLineEndPointPivotLowIter != pivotLows->end(); trendLineEndPointPivotLowIter++)
					{
						// Create Linear equations defining the upper and lower trend-lines
						PeriodValCltn::iterator secondPivotLowIter = (*trendLineEndPointPivotLowIter)->lowestLowIter();

						ChartSegmentPtr upperTrendLine(new ChartSegment(chartVals->perValCltn(),
								firstPivotHighIter,secondPivotHighIter,
								PeriodValueRefPtr(new HighPeriodValueRef())));
						ChartSegmentPtr lowerTrendLine(new ChartSegment(chartVals->perValCltn(),
								firstPivotLowIter,secondPivotLowIter,
								PeriodValueRefPtr(new LowPeriodValueRef)));

						if(validTrendLines(upperTrendLine,lowerTrendLine))
						{
							// The starting point to test for the end of the pattern match occurs after
							// the 2nd pivot high or the 2nd pivot low, whichever has a date farthest in the future.
							PeriodValCltn::iterator endPatternMatchBeginIter =
									upperTrendLine->lastPeriodVal().periodTime() > lowerTrendLine->lastPeriodVal().periodTime()?
											upperTrendLine->perValSegment()->segEnd():lowerTrendLine->perValSegment()->segEnd();

							assert((*endPatternMatchBeginIter).perValIndex() > (*firstPivotHighIter).perValIndex());
							unsigned int currNumPeriods = (*endPatternMatchBeginIter).perValIndex() -
									(*firstPivotHighIter).perValIndex();

							UnsignedIntRange validPeriodRange( minPatternPeriods(upperTrendLine,lowerTrendLine),
									maxPatternPeriods(upperTrendLine,lowerTrendLine));
							for(PeriodValCltn::iterator endPatternMatchIter = endPatternMatchBeginIter;
									(endPatternMatchIter != chartVals->segEnd()) && (currNumPeriods<=validPeriodRange.maxVal());
									endPatternMatchIter++, currNumPeriods++)
							{
								if(validPeriodRange.valueWithinRange(currNumPeriods))
								{
                                    WedgeMatchValidationInfo wedgeValidationInfo(chartVals,
                                             upperTrendLine,lowerTrendLine,endPatternMatchIter);
                                    if(validWedgePatternMatch(wedgeValidationInfo))
                                    {
                                        PatternMatchPtr foundPatternMatch = findPatternMatch(wedgeValidationInfo);
                                        if(foundPatternMatch)
                                        {
                                            wedgeMatches->push_back(foundPatternMatch);
                                        }

                                    } // If the pattern match is well formed.
								} // If the period length is within the valid range

								assert(((*endPatternMatchIter).perValIndex()-(*firstPivotHighIter).perValIndex())==currNumPeriods);
							} // for each potential ending to the match

						} // if valid trend line

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time

		} // for each 2nd pivot high (forming the upper trend-line)

	} // for each starting/first pivot high (forming the upper trend-line)

	return wedgeMatches;

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

