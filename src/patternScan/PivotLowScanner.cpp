/*
 * PivotLowScanner.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#include <PivotLowScanner.h>
#include "VScanner.h"
#include "MultiPatternScanner.h"
#include "PatternMatchFilter.h"
#include "CupScanner.h"
#include "PatternMatchValidator.h"
#include "ORPatternMatchValidator.h"
#include "PatternLengthLessThanEqual.h"
#include "PeriodValueRef.h"
#include "ChartSegment.h"
#include "PerValCltnSlidingWindow.h"
#include "DebugLog.h"

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

    /*
    PatternScannerPtr vPivotLowScanner(new VScanner(pivotLowMaxTrendLineDistancePerc_));
    MultiPatternScanner vPivotLowMultiPatternScanner(vPivotLowScanner);
    PatternMatchListPtr vShapedPivotLows = vPivotLowMultiPatternScanner.scanPatternMatches(chartVals);
    allPivots->insert(allPivots->end(),vShapedPivotLows->begin(),vShapedPivotLows->end());
*/

/* The code below is experimental/WIP code for different algorithms for pivot scanning,
 * using either the cup shaped scanner or a sliding window when pivots occur.
 */
/*
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

    unsigned int pivotWindowLen = 6;
    if(PerValCltnSlidingWindow::windowFitsWithinRange(pivotWindowLen,chartVals->segBegin(),chartVals->segEnd()))
    {
        PerValCltnSlidingWindow slidingPivotTestWindow(pivotWindowLen,chartVals->segBegin(),chartVals->segEnd());

        while(!slidingPivotTestWindow.windowAtEnd())
        {
            if((slidingPivotTestWindow.middleVal().low() <
                  slidingPivotTestWindow.firstVal().low()) &&
                    (slidingPivotTestWindow.middleVal().low() <
                     slidingPivotTestWindow.lastVal().low())
                   )
             {

                DEBUG_MSG("PivotLowScanner: found pivot low: "
                        << "LHS=" << slidingPivotTestWindow.firstVal()
                        << "Middle (pivot)=" << slidingPivotTestWindow.middleVal()
                        << "RHS=" << slidingPivotTestWindow.lastVal());
                ChartSegmentPtr pivotSeg(new ChartSegment(chartVals->perValCltn(),
                        slidingPivotTestWindow.windowFirst(),slidingPivotTestWindow.windowLast(),
                        PeriodValueRefPtr(new TypicalPricePeriodValueRef())));
                PatternMatchPtr pivotMatch(new PatternMatch(pivotSeg));
                allPivots->push_back(pivotMatch);

              }
              slidingPivotTestWindow.advanceWindow();

        }
    }

    PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestLowestLow(allPivots);

    DEBUG_MSG("PivotLowScanner: num pivot lows: " << sortedUniquePivots->size());
	for(PatternMatchList::iterator matchIter = sortedUniquePivots->begin(); matchIter != sortedUniquePivots->end(); matchIter++)
	{
        DEBUG_MSG("PivotLowScanner: pivot low: "
				<< "time=" << (*matchIter)->lowestLowVal().periodTime()
				<< "(psuedo) x val=" << (*matchIter)->lowestLowVal().pseudoXVal()
                << ", lowest low=" << (*matchIter)->lowestLow());
	}

	return sortedUniquePivots;

}


PivotLowScanner::~PivotLowScanner() {
}

