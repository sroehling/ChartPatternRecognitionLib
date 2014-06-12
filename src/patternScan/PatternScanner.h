/*
 * PatternScanner.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNSCANNER_H_
#define PATTERNSCANNER_H_

#include "SegmentConstraint.h"
#include "SegmentListConstraint.h"
#include "PatternMatch.h"
#include "PatternMatchValidator.h"

class PatternScanner {
private:
	SegmentConstraintPtr segmentConstraint_;
	SegmentListConstraintPtr segmentListConstraint_;
	PatternMatchValidatorPtr patternMatchValidator_;
	unsigned int minSegmentLength_;
	unsigned int maxSegmentLength_;

	void splitPeriodVals(const PeriodValCltn &srcCltn,
				unsigned int splitPos, PeriodValCltn &beforeSplitPos,
				PeriodValCltn &afterSplitPos);
	PatternMatchListPtr scanPatternMatches(
			const ChartSegmentList &leadingSegments,
			const PeriodValCltn &remainingVals);
public:
	PatternScanner(const SegmentConstraintPtr &segmentConstraint,
				const SegmentListConstraintPtr &segmentListConstraint,
				const PatternMatchValidatorPtr &patternMatchValidator);

	// Scan to find a pattern match(es) starting at the first value in chartVals.
	// This is the main entry point for scanning (i.e., the top level of recursion).
	PatternMatchListPtr scanPatternMatches(const PeriodValCltn &chartVals);

	virtual ~PatternScanner();
};

#endif /* PATTERNSCANNER_H_ */
