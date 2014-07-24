/*
 * WedgeScannerEngine.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <WedgeScannerEngine.h>
#include "VScanner.h"
#include "InvertedVScanner.h"
#include "PeriodValSegment.h"
#include "PatternMatchFilter.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFindPredicate.h"
#include "PeriodValueRef.h"

WedgeScannerEngine::WedgeScannerEngine() {
	pivotLowMaxTrendLineDistancePerc_ = 3.0;
}

PatternMatchListPtr WedgeScannerEngine::scanPivotHighs(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotHighScanner(new InvertedVScanner(pivotLowMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotHighMultiPatternScanner(pivotHighScanner);
	PatternMatchListPtr pivotHighs = pivotHighMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(pivotHighs);
	return sortedUniquePivots;
}

PatternMatchListPtr WedgeScannerEngine::scanPivotLows(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotLowScanner(new VScanner(pivotLowMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotLowMultiPatternScanner(pivotLowScanner);
	PatternMatchListPtr pivotLows = pivotLowMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestLowestLow(pivotLows);
	return sortedUniquePivots;

}

PatternMatchListPtr WedgeScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr wedgeMatches(new PatternMatchList());

	// Unlike the PatternScannerEngine, this scanner will return pattern matches starting
	// anywhere from the beginning of chartVals to the end.

	// The scanning starts by getting a list of pivot highs and lows within chartVals.
	// These pivots serve as anchor points for drawing candidate trend lines for the
	// pattern matches.
	PatternMatchListPtr pivotHighs = scanPivotHighs(chartVals);
	PatternMatchListPtr pivotLows = scanPivotLows(chartVals);

	// Each of the pivot highs serves as a potential starting point for the pattern match.
	for(PatternMatchList::iterator startMatchPivotHighIter = pivotHighs->begin();
			startMatchPivotHighIter != pivotHighs->end(); startMatchPivotHighIter++)
	{

		PatternMatchList::iterator remainingPivotsBegin = startMatchPivotHighIter;
		remainingPivotsBegin++;
		// The remaining pivot highs are candidates for drawing trend lines to form the pattern.
		for(PatternMatchList::iterator trendLineEndPointPivotHighIter = remainingPivotsBegin;
				trendLineEndPointPivotHighIter != pivotHighs->end(); trendLineEndPointPivotHighIter++)
		{
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
					// TODO - Test if depth from the pivot high to pivot low satisfies the depth criterion.
					PatternMatchList::iterator remainingPivotLowsBegin = startPivotLowIter;
					remainingPivotLowsBegin++;
					for(PatternMatchList::iterator trendLineEndPointPivotLowIter = remainingPivotLowsBegin;
							trendLineEndPointPivotLowIter != pivotLows->end(); trendLineEndPointPivotLowIter++)
					{
						// Create Linear equations defining the upper and lower trend-lines
						PeriodValSegmentPtr pivotHighSeg(new PeriodValSegment(chartVals->perValCltn(),
								(*startMatchPivotHighIter)->highestHighIter(),
								(*trendLineEndPointPivotHighIter)->highestHighIter()));
						LinearEquationPtr upperTrendLineEq = pivotHighSeg->segmentEquation(HighPeriodValueRef());

						PeriodValSegmentPtr pivotLowSeg(new PeriodValSegment(chartVals->perValCltn(),
								(*startPivotLowIter)->lowestLowIter(),
								(*trendLineEndPointPivotLowIter)->lowestLowIter()));
						LinearEquationPtr lowerTrendLineEq = pivotLowSeg->segmentEquation(LowPeriodValueRef());

						// TODO - Validate the slopes of the trend-lines
						// TODO - Calculate the apex between upper and lower trend-lines.
						// TODO - Validate the distance to the apex is within the range for a pattern match
						// TODO - For reporting a pattern match, validate the matching done thus far is far
						// 	enough along towards the apex (e.g., 60-70%).
						// TODO - Iterate through each point between the starting pivot high and the apex
						// (or end, whichever comes first), and check that each value and a % of overall values
						// is between the two equations.

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time
		}
	}

	return wedgeMatches; // stubbed-out.

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

