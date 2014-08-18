/*
 * PatternScanner.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNSCANNERENGINE_H_
#define PATTERNSCANNERENGINE_H_

#include "SegmentConstraint.h"
#include "SegmentListConstraint.h"
#include "PatternMatch.h"
#include "PatternMatchValidator.h"
#include "PeriodValSegment.h"
#include "UnsignedIntRange.h"
#include "PatternScanner.h"

class PatternScannerEngine: public PatternScanner {
private:
	SegmentConstraintPtr segmentConstraint_;
	SegmentListConstraintPtr segmentListConstraint_;
	PatternMatchValidatorPtr patternMatchValidator_;

    UnsignedIntRange segmentLengthRange_;

	PatternMatchListPtr scanPatternMatches(
			const ChartSegmentList &leadingSegments,
			const PeriodValSegmentPtr &remainingVals) const;
public:
	PatternScannerEngine(const SegmentConstraintPtr &segmentConstraint,
				const SegmentListConstraintPtr &segmentListConstraint,
				const PatternMatchValidatorPtr &patternMatchValidator);

    PatternScannerEngine(const SegmentConstraintPtr &segmentConstraint,
            const SegmentListConstraintPtr &segmentListConstraint,
            const PatternMatchValidatorPtr &patternMatchValidator,
            const UnsignedIntRange &segmentLengthRange);


    static const UnsignedIntRange DEFAULT_SCAN_SEGMENT_LENGTH_RANGE;

	// Scan to find a pattern match(es) starting at the first value in chartVals.
	// This is the main entry point for scanning (i.e., the top level of recursion).
	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~PatternScannerEngine();
};



#endif /* PATTERNSCANNERENGINE_H_ */
