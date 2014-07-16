/*
 * InvertedVScanner.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef INVERTEDVSCANNER_H_
#define INVERTEDVSCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScanner.h"
#include "PatternMatchValidator.h"
#include "CompositePatternMatchValidatorFactory.h"


class InvertedVScanner {
private:
	CompositePatternMatchValidatorFactory downTrendValidatorFactory_;
	CompositePatternMatchValidatorFactory overallValidatorFactory_;

public:

	CompositePatternMatchValidatorFactory &upTrendValidatorFactory() { return downTrendValidatorFactory_; }
	CompositePatternMatchValidatorFactory &overallValidatorFactory() { return overallValidatorFactory_; }

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	InvertedVScanner();
	virtual ~InvertedVScanner();
};

#endif /* INVERTEDVSCANNER_H_ */
