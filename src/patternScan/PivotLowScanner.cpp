/*
 * PivotLowScanner.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#include <PivotLowScanner.h>
#include "VScanner.h"
#include "MultiPatternScanner.h"
#include <boost/log/trivial.hpp>
#include "PatternMatchFilter.h"
#include "CupScanner.h"
#include "PatternMatchValidator.h"
#include "ORPatternMatchValidator.h"
#include "PatternLengthLessThanEqual.h"
#include "PeriodValueRef.h"
#include "ChartSegment.h"

PivotLowScanner::PivotLowScanner() {
	pivotLowMaxTrendLineDistancePerc_ = 3.0;
}

PivotLowScanner::PivotLowScanner(double maxDistanceToTrendLinesPerc)
    : pivotLowMaxTrendLineDistancePerc_(maxDistanceToTrendLinesPerc)
{
    assert(maxDistanceToTrendLinesPerc > 0.0);
}


PatternMatchListPtr PivotLowScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr allPivots(new PatternMatchList());

    PatternScannerPtr vPivotLowScanner(new VScanner(pivotLowMaxTrendLineDistancePerc_));
    MultiPatternScanner vPivotLowMultiPatternScanner(vPivotLowScanner);
    PatternMatchListPtr vShapedPivotLows = vPivotLowMultiPatternScanner.scanPatternMatches(chartVals);
    allPivots->insert(allPivots->end(),vShapedPivotLows->begin(),vShapedPivotLows->end());
/* The code below is experimental/WIP code for different algorithms for pivot scanning,
 * using either the cup shaped scanner or a sliding window when pivots occur.
     PatternMatchValidatorList finalValidators;
    finalValidators.push_back(PatternMatchValidatorPtr(new PatternLengthLessThanEqual(20)));
    PatternMatchValidatorPtr overallValidator(new ORPatternMatchValidator(finalValidators));
    CupScanner *cupScanner = new CupScanner();
    cupScanner->overallValidatorFactory().addStaticValidator(overallValidator);

    PatternScannerPtr uShapedPivotLowScanner(cupScanner);
    MultiPatternScanner uPivotLowMultiPatternScanner(uShapedPivotLowScanner);
    PatternMatchListPtr uShapedPivotLows = uPivotLowMultiPatternScanner.scanPatternMatches(chartVals);
     allPivots->insert(allPivots->end(),uShapedPivotLows->begin(),uShapedPivotLows->end());
*/

    /*
    unsigned int pivotLen = 3;
    PeriodValCltn::iterator pivotLHSIter = chartVals->segBegin();
    PeriodValCltn::iterator pivotMiddleIter = pivotLHSIter;
    std::advance(pivotMiddleIter,pivotLen);
    PeriodValCltn::iterator pivotRHSIter = pivotMiddleIter;
    std::advance(pivotRHSIter,pivotLen);

    for(pivotRHSIter = pivotRHSIter; pivotRHSIter != chartVals->segEnd();
        pivotLHSIter++,pivotMiddleIter++,pivotRHSIter++)
    {
        if(((*pivotMiddleIter).low() < (*pivotLHSIter).low()) &&
               ((*pivotMiddleIter).low() < (*pivotRHSIter).low()))
        {
            BOOST_LOG_TRIVIAL(debug) << "PivotLowScanner: pivot low: "
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
*/

    PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestLowestLow(allPivots);

    BOOST_LOG_TRIVIAL(debug) << "PivotLowScanner: num pivot lows: " << sortedUniquePivots->size() << std::endl;
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
		BOOST_LOG_TRIVIAL(debug) << "PivotLowScanner: pivot low: "
				<< "time=" << (*matchIter)->lowestLowVal().periodTime()
				<< "(psuedo) x val=" << (*matchIter)->lowestLowVal().pseudoXVal()
				<< ", lowest low=" << (*matchIter)->lowestLow() << std::endl;
	}

	return sortedUniquePivots;

}


PivotLowScanner::~PivotLowScanner() {
}

