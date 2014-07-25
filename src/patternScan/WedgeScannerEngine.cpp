/*
 * WedgeScannerEngine.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
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

	BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num pivot highs: " << sortedUniquePivots->size() << std::endl;
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: pivot high: " << (*matchIter)->highestHigh() << std::endl;
	}

	return sortedUniquePivots;
}

PatternMatchListPtr WedgeScannerEngine::scanPivotLows(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotLowScanner(new VScanner(pivotLowMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotLowMultiPatternScanner(pivotLowScanner);
	PatternMatchListPtr pivotLows = pivotLowMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestLowestLow(pivotLows);

	BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num pivot lows: " << sortedUniquePivots->size() << std::endl;
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: pivot low: " << (*matchIter)->lowestLow() << std::endl;
	}

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



						// Calculate the intersection between upper and lower trend-lines.
						// This is where the pattern ends.
						if(lowerTrendLineEq->slope() != upperTrendLineEq->slope())
						{
							// Only continue if the lower and upper trend-line have different slopes.
							// Otherwise they'll never intersect. This being said, a similar scanning
							// algorithm may be appropriate for channels.
							XYCoord trendlineIntercept = lowerTrendLineEq->intercept(*upperTrendLineEq);

							// Only continue if the intercept occurs after the first pivot high's "pseudo X" value
							// (i.e., the unique numerical value assigned for each PeriodVal's date). If the intercept
							// is before the first pivot high, then the lines are angled away from each other, and
							// we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
							// at some point, but not here).
							double firstPivotHighXVal = (*startMatchPivotHighIter)->highestHighVal().pseudoXVal();
							if(trendlineIntercept.x() > (*startMatchPivotHighIter)->highestHighVal().pseudoXVal())
							{
								// TODO - Besides validating the trendlines intersect after the first pivot high,
								// validate the slopes of the individual trendlines are within the tolerances for
								// an acceptable pattern match (e.g., we don't want both to have a steep negative slope,
								// even if they do intercept).

								double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;
								BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: " << numPeriodsToIntercept << std::endl;

								// TODO - Validate the distance to the apex is within the range for a pattern match

								// TODO - Iterate through each point between the starting pivot high and the apex
								// (or end, whichever comes first), and check that each value and a % of overall values
								// is between the two equations.

								// TODO - For reporting a pattern match, validate the matching done thus far is far
								// 	enough along towards the apex (e.g., 60-70%).


						} // if trendline intercept occurs after the pivot high

						} // If lower and upper trend line have different slopes (i.e., they intersect)

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time
		}
	}

	return wedgeMatches; // stubbed-out.

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

