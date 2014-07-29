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
#include "PeriodValSegment.h"
#include "ChartSegment.h"
#include "DoubleRange.h"

WedgeScannerEngine::WedgeScannerEngine() {
	pivotLowMaxTrendLineDistancePerc_ = 3.0;
	minPercDistanceToUpperLowerTrendlineIntercept_ = 0.6;
	minPercValsBetweenTrendlines_ = 0.85;
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
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: pivot high:"
				<< " time=" << (*matchIter)->highestHighVal().periodTime()
				<< " (psuedo) x val=" << (*matchIter)->highestHighVal().pseudoXVal()
				<< ", highest high=" << (*matchIter)->highestHigh() << std::endl;
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
		BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: pivot low: "
				<< "time=" << (*matchIter)->lowestLowVal().periodTime()
				<< "(psuedo) x val=" << (*matchIter)->lowestLowVal().pseudoXVal()
				<< ", lowest low=" << (*matchIter)->lowestLow() << std::endl;
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

						XYCoord startUpperTrendline((*firstPivotHighIter).pseudoXVal(),(*firstPivotHighIter).high());
						XYCoord endUpperTrendline((*secondPivotHighIter).pseudoXVal(),(*secondPivotHighIter).high());
						LinearEquationPtr upperTrendLineEq(new LinearEquation(startUpperTrendline,endUpperTrendline));

						XYCoord startLowerTrendline((*firstPivotLowIter).pseudoXVal(),(*firstPivotLowIter).low());
						XYCoord endLowerTrendline((*secondPivotLowIter).pseudoXVal(),(*secondPivotLowIter).low());
						LinearEquationPtr lowerTrendLineEq(new LinearEquation(startLowerTrendline,endLowerTrendline));

						// Calculate the intersection between upper and lower trend-lines.
						// This is where the pattern ends.
						if(lowerTrendLineEq->slope() != upperTrendLineEq->slope())
						{
							// Only continue if the lower and upper trend-line have different slopes.
							// Otherwise they'll never intersect. This being said, a similar scanning
							// algorithm may be appropriate for channels.
							XYCoord trendlineIntercept = lowerTrendLineEq->intercept(*upperTrendLineEq);
							BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: "
									<< " upper trend line: start="<< startUpperTrendline << " end=" << endUpperTrendline
									<< " lower trend line: start="<< startLowerTrendline << " end=" << endLowerTrendline
									<< " intercept " << trendlineIntercept << std::endl;

							// Only continue if the intercept occurs after the first pivot high's "pseudo X" value
							// (i.e., the unique numerical value assigned for each PeriodVal's date). If the intercept
							// is before the first pivot high, then the lines are angled away from each other, and
							// we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
							// at some point, but not here).
							double firstPivotHighXVal = (*startMatchPivotHighIter)->highestHighVal().pseudoXVal();
							if(trendlineIntercept.x() > firstPivotHighXVal)
							{
								// TODO - Besides validating the trendlines intersect after the first pivot high,
								// validate the slopes of the individual trendlines are within the tolerances for
								// an acceptable pattern match (e.g., we don't want both to have a steep negative slope,
								// even if they do intercept).

								double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;
								BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: "
										<< numPeriodsToIntercept << std::endl;


								// The starting point to test for the end of the pattern match occurs after
								// the 2nd pivot high or the 2nd pivot low, whichever has a date farthest in the future.
								PeriodValCltn::iterator endPatternMatchBeginIter =
										(*secondPivotHighIter).periodTime() > (*secondPivotLowIter).periodTime()?
												secondPivotHighIter:secondPivotLowIter;

								// Starting at endPatternMatchBeginIter then ending at the x value of the trend line intercept or
								// the last value in chartVals (whichever comes first), test each potential end point for a
								// completed pattern match.
								PeriodValCltn::iterator prevValIter = endPatternMatchBeginIter;
								// endPatternMatchIter is setup based upon the 2nd pivot high or low, so it cannot start at segBegin()
								assert(prevValIter != chartVals->segBegin());
								prevValIter--;
								for(PeriodValCltn::iterator endPatternMatchIter = endPatternMatchBeginIter;
										(endPatternMatchIter != chartVals->segEnd()) && ((*endPatternMatchIter).pseudoXVal() <= trendlineIntercept.x());
										endPatternMatchIter++, prevValIter++)
								{

									// Validate the matching done thus far is far enough along towards the intercept (e.g., 60-70%).
									DoubleRange patternRange(firstPivotHighXVal,trendlineIntercept.x());
									double percentOfPatternLengthCovered = patternRange.percentWithinRange((*endPatternMatchIter).pseudoXVal());
									BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: validating match length vs pattern length: "
												<< patternRange << " %covered=" << percentOfPatternLengthCovered << std::endl;

									if(percentOfPatternLengthCovered >= minPercDistanceToUpperLowerTrendlineIntercept_)
									{

										if(upperTrendLineEq->belowLine((*prevValIter).closeCoord()) &&
												upperTrendLineEq->aboveLine((*endPatternMatchIter).closeCoord()))
										{

											BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: upper trend line crossover: "
														<< "prev val=" << (*prevValIter).closeCoord()
														<< ", curr val=" << (*endPatternMatchIter).closeCoord()
														<< ", curr period val=" << (*endPatternMatchIter) << std::endl;

											// PeriodValSegment works like regular STL iterator, in that iteration over
											// the segment includes everything except segEnd. So, to include the value
											// pointed to by endPatternMatchIter, we need iterate one beyond endPatternMatchIter.
											PeriodValCltn::iterator segEnd = endPatternMatchIter;
											segEnd++;

											PeriodValSegmentPtr patternSeg(new PeriodValSegment(chartVals->perValCltn(),
													firstPivotHighIter,segEnd));
											ChartSegmentPtr chartSeg(new ChartSegment(patternSeg));
											PatternMatchPtr patternMatch(new PatternMatch(chartSeg));
											wedgeMatches->push_back(patternMatch);
											// TODO Based upon minPercValsBetweenTrendlines_, Check that each value and a % of overall values
											// is between the two equations.

										} // if the closing price crosses over the upper trend line (upside break-out)

									} // If the X value for endPatternMatchIter is far enough along towards the upper&lower trend line intercept

								} // For each potential ending to the pattern match

							} // if trendline intercept occurs after the pivot high

						} // If lower and upper trend line have different slopes (i.e., they intersect)

					} // for each candidate pivot low to serve as the end-point for drawing the lower trend-line

				} // for each pivot low after the starting pivot high to serve as the start point for lower trend-line.

			} // If a pivot low is found after the starting pivot high's time

		} // for each 2nd pivot high (forming the upper trend-line)

	} // for each starting/first pivot high (forming the upper trend-line)

	return wedgeMatches;

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

