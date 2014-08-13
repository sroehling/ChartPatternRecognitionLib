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
#include "CompositePatternMatchValidatorFactory.h"
#include "PatternScanner.h"

class CupScanner: public PatternScanner {
private:
	CompositePatternMatchValidatorFactory upTrendValidatorFactory_;
	CompositePatternMatchValidatorFactory overallValidatorFactory_;
public:
	CupScanner();

	CompositePatternMatchValidatorFactory &upTrendValidatorFactory() { return upTrendValidatorFactory_; }
	CompositePatternMatchValidatorFactory &overallValidatorFactory() { return overallValidatorFactory_; }

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~CupScanner() {}
};

#endif /* CUPSCANNER_H_ */
