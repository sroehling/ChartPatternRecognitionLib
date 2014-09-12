/*
 * MultiPatternScanner.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef MULTIPATTERNSCANNER_H_
#define MULTIPATTERNSCANNER_H_

#include "PatternScanner.h"
#include "PeriodValSegment.h"

class MultiPatternScanner {
private:
	PatternScannerPtr singlePatternScanner_;

    void scanFromStartingPoint(const PatternMatchListPtr &matches,
          const PeriodValCltn::iterator &scanStartIter, const PeriodValSegmentPtr &chartVals) const;

public:
	MultiPatternScanner(const PatternScannerPtr &singlePatternScanner);

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

    PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals,
                 const PeriodValCltnIterListPtr &scanStartingPoints) const;

    // Scan for all the matches starting anywhere in chartVals, but filter the matched
    // patterns to those not ending on the same date and the earliest start time.
    PatternMatchListPtr scanUniquePatternMatches(const PeriodValSegmentPtr &chartVals) const;

    PatternMatchListPtr scanUniquePatternMatches(const PeriodValSegmentPtr &chartVals,
                 const PeriodValCltnIterListPtr &scanStartingPoints) const;


	virtual ~MultiPatternScanner();
};

#endif /* MULTIPATTERNSCANNER_H_ */
