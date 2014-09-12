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

void MultiPatternScanner::scanFromStartingPoint(const PatternMatchListPtr &matches,
      const PeriodValCltn::iterator &scanStartIter, const PeriodValSegmentPtr &chartVals) const
{
    PeriodValSegmentPtr subSegmentForScanning(new PeriodValSegment(chartVals->perValCltn(),
                    scanStartIter, chartVals->segEnd()));
    PatternMatchListPtr currSubSegmentMatches =
            singlePatternScanner_->scanPatternMatches(subSegmentForScanning);

    matches->insert(matches->end(),
            currSubSegmentMatches->begin(), currSubSegmentMatches->end());

}

PatternMatchListPtr MultiPatternScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr allSubSegmentMatches(new PatternMatchList());

	// Scan for patterns through chartVals. Each position within chartVals serves as an alternate
	// starting point for scanning.
	for(PeriodValCltn::iterator scanStartIter = chartVals->segBegin();
			scanStartIter != chartVals->segEnd(); scanStartIter++)
	{
        scanFromStartingPoint(allSubSegmentMatches,scanStartIter,chartVals);
	}

	return allSubSegmentMatches;
}

PatternMatchListPtr MultiPatternScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals,
                                                            const PeriodValCltnIterListPtr &scanStartingPoints) const
{
    PatternMatchListPtr allSubSegmentMatches(new PatternMatchList());

    for(PeriodValCltnIterList::iterator scanStartIter = scanStartingPoints->begin();
        scanStartIter != scanStartingPoints->end(); scanStartIter++)
    {
        scanFromStartingPoint(allSubSegmentMatches,*scanStartIter,chartVals);

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


PatternMatchListPtr MultiPatternScanner::scanUniquePatternMatches(const PeriodValSegmentPtr &chartVals,
             const PeriodValCltnIterListPtr &scanStartingPoints) const
{
    PatternMatchListPtr unfilteredMatches = scanPatternMatches(chartVals,scanStartingPoints);
    PatternMatchListPtr uniqueFilteredMatches =
            patternMatchFilter::filterUniqueLongestPatternSameEndDate(unfilteredMatches);
    return uniqueFilteredMatches;

}




MultiPatternScanner::~MultiPatternScanner() {
}

