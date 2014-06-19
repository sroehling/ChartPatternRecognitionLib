/*
 * PatternScanner.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNSCANNER_H_
#define PATTERNSCANNER_H_

#include <boost/shared_ptr.hpp>

#include "SegmentConstraint.h"
#include "SegmentListConstraint.h"
#include "PatternMatch.h"
#include "PatternMatchValidator.h"
#include "PeriodValSegment.h"

class PatternScanner {
private:
	SegmentConstraintPtr segmentConstraint_;
	SegmentListConstraintPtr segmentListConstraint_;
	PatternMatchValidatorPtr patternMatchValidator_;
	unsigned int minSegmentLength_;
	unsigned int maxSegmentLength_;

	PatternMatchListPtr scanPatternMatches(
			const ChartSegmentList &leadingSegments,
			const PeriodValSegmentPtr &remainingVals);
public:
	PatternScanner(const SegmentConstraintPtr &segmentConstraint,
				const SegmentListConstraintPtr &segmentListConstraint,
				const PatternMatchValidatorPtr &patternMatchValidator);

	// Scan to find a pattern match(es) starting at the first value in chartVals.
	// This is the main entry point for scanning (i.e., the top level of recursion).
	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals);

	virtual ~PatternScanner();
};

typedef boost::shared_ptr<PatternScanner> PatternScannerPtr;


#endif /* PATTERNSCANNER_H_ */
