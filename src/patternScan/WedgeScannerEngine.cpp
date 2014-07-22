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

			// Get the first pivot low which starts after the starting pivot high. This
			// first pivot low serves as the candidate starting point to draw the corresponding
			// lower trend line.
		}
	}

	return wedgeMatches; // stubbed-out.

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

