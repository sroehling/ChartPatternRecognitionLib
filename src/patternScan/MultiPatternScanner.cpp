/*
 * MultiPatternScanner.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <MultiPatternScanner.h>
#include "PatternMatchFilter.h"

MultiPatternScanner::MultiPatternScanner(const PatternScannerPtr &singlePatternScanner)
: singlePatternScanner_(singlePatternScanner)
{
}

PatternMatchListPtr MultiPatternScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr allSubSegmentMatches(new PatternMatchList());

	// Scan for patterns through chartVals. Each position within chartVals serves as an alternate
	// starting point for scanning.
	for(PeriodValCltn::iterator scanStartIter = chartVals->segBegin();
			scanStartIter != chartVals->segEnd(); scanStartIter++)
	{
		PeriodValSegmentPtr subSegmentForScanning(new PeriodValSegment(chartVals->perValCltn(),
						scanStartIter, chartVals->segEnd()));
		PatternMatchListPtr currSubSegmentMatches =
				singlePatternScanner_->scanPatternMatches(subSegmentForScanning);

		allSubSegmentMatches->insert(allSubSegmentMatches->end(),
				currSubSegmentMatches->begin(), currSubSegmentMatches->end());
	}

	return allSubSegmentMatches;
}

PatternMatchListPtr MultiPatternScanner::scanUniquePatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr unfilteredMatches = scanPatternMatches(chartVals);
    PatternMatchListPtr uniqueFilteredMatches =
            patternMatchFilter::filterUniqueLongestPatternSameEndDate(unfilteredMatches);
    return uniqueFilteredMatches;
}




MultiPatternScanner::~MultiPatternScanner() {
}

