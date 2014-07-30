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

PivotLowScanner::PivotLowScanner() {
	pivotLowMaxTrendLineDistancePerc_ = 3.0;
}

PatternMatchListPtr PivotLowScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotLowScanner(new VScanner(pivotLowMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotLowMultiPatternScanner(pivotLowScanner);
	PatternMatchListPtr pivotLows = pivotLowMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestLowestLow(pivotLows);

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

