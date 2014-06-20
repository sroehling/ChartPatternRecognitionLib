/*
 * CupScanner.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#ifndef CUPSCANNER_H_
#define CUPSCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScannerEngine.h"


class CupScanner {
public:
	CupScanner();

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~CupScanner();
};

#endif /* CUPSCANNER_H_ */
