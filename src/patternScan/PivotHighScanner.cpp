/*
 * PivotHighScanner.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#include <PivotHighScanner.h>
#include "InvertedVScanner.h"
#include <boost/log/trivial.hpp>
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"


PivotHighScanner::PivotHighScanner() {
	pivotHighMaxTrendLineDistancePerc_ = 3.0;
}

PivotHighScanner::PivotHighScanner(double maxTrendLineDistancePerc)
: pivotHighMaxTrendLineDistancePerc_(maxTrendLineDistancePerc)
{
    assert(maxTrendLineDistancePerc > 0.0);
}

PatternMatchListPtr PivotHighScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotHighScanner(new InvertedVScanner(pivotHighMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotHighMultiPatternScanner(pivotHighScanner);
	PatternMatchListPtr pivotHighs = pivotHighMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(pivotHighs);


    /* The code below is experimental code for matching pivots. Rather than progressively scanning for
     * using the InvertedVScanner, use a sliding window to determine where pivots have taken place.

    PatternMatchListPtr allPivots(new PatternMatchList());

    unsigned int pivotLen = 3;
    PeriodValCltn::iterator pivotLHSIter = chartVals->segBegin();
    PeriodValCltn::iterator pivotMiddleIter = pivotLHSIter;
    std::advance(pivotMiddleIter,pivotLen);
    PeriodValCltn::iterator pivotRHSIter = pivotMiddleIter;
    std::advance(pivotRHSIter,pivotLen);

    for(pivotRHSIter = pivotRHSIter; pivotRHSIter != chartVals->segEnd();
        pivotLHSIter++,pivotMiddleIter++,pivotRHSIter++)
    {
        if(((*pivotMiddleIter).high() > (*pivotLHSIter).high()) &&
               ((*pivotMiddleIter).high() > (*pivotRHSIter).high()))
        {
            BOOST_LOG_TRIVIAL(debug) << "PivotHighScanner: pivot high: "
                    << "LHS=" << (*pivotLHSIter)
                    << "Middle (pivot)=" << (*pivotMiddleIter)
                    << "RHS=" << (*pivotRHSIter) << std::endl;
            PeriodValCltn::iterator pivotEndIter = pivotRHSIter;
            pivotEndIter++;
            ChartSegmentPtr pivotSeg(new ChartSegment(chartVals->perValCltn(),
                    pivotLHSIter,pivotEndIter,
                    PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
            PatternMatchPtr pivotMatch(new PatternMatch(pivotSeg));
            allPivots->push_back(pivotMatch);
        }
    }

    PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(allPivots);
*/


	BOOST_LOG_TRIVIAL(debug) << "PivotHighScanner: num pivot highs: " << sortedUniquePivots->size() << std::endl;
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
		BOOST_LOG_TRIVIAL(debug) << "PivotHighScanner: pivot high:"
				<< " time=" << (*matchIter)->highestHighVal().periodTime()
				<< " (psuedo) x val=" << (*matchIter)->highestHighVal().pseudoXVal()
				<< ", highest high=" << (*matchIter)->highestHigh() << std::endl;
	}

	return sortedUniquePivots;
}


PivotHighScanner::~PivotHighScanner() {
}

