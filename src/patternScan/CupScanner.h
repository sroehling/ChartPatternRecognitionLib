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
    CompositePatternMatchValidatorFactory downTrendValidatorFactory_;
    CompositePatternMatchValidatorFactory flatBottomValidatorFactory_;
    CompositePatternMatchValidatorFactory upTrendValidatorFactory_;
	CompositePatternMatchValidatorFactory overallValidatorFactory_;

    double trendlineMaxDistancePerc_;
    unsigned int minTrendLineSegmentLength_;

    void initConstraints();

public:
	CupScanner();
    CupScanner(unsigned int minTrendLineSegmentLength);

    CompositePatternMatchValidatorFactory &downTrendValidatorFactory() { return downTrendValidatorFactory_; }
    CompositePatternMatchValidatorFactory &flatBottomValidatorFactory() { return flatBottomValidatorFactory_; }
    CompositePatternMatchValidatorFactory &upTrendValidatorFactory() { return upTrendValidatorFactory_; }
	CompositePatternMatchValidatorFactory &overallValidatorFactory() { return overallValidatorFactory_; }


	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~CupScanner() {}
};

#endif /* CUPSCANNER_H_ */
