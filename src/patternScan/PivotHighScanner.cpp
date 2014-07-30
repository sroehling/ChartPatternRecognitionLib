/*
 * PivotHighScanner.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: sroehling
 */

#include <PivotHighScanner.h>
#include "InvertedVScanner.h"
#include <boost/log/trivial.hpp>


PivotHighScanner::PivotHighScanner() {
	pivotHighMaxTrendLineDistancePerc_ = 3.0;
}

PatternMatchListPtr PivotHighScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr pivotHighScanner(new InvertedVScanner(pivotHighMaxTrendLineDistancePerc_));
	MultiPatternScanner pivotHighMultiPatternScanner(pivotHighScanner);
	PatternMatchListPtr pivotHighs = pivotHighMultiPatternScanner.scanPatternMatches(chartVals);
	PatternMatchListPtr sortedUniquePivots = patternMatchFilter::filterUniqueAndLongestHighestHigh(pivotHighs);

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

