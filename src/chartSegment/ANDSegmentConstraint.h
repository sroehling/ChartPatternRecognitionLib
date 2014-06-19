/*
 * ANDSegmentContraint.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#ifndef ANDSEGMENTCONSTRAINT_H_
#define ANDSEGMENTCONSTRAINT_H_

#include <SegmentConstraint.h>

// SegmentContraint where validSegment() performs a boolean
// AND on all the other contraints in the given
// SegmentConstraintList.
class ANDSegmentConstraint: public SegmentConstraint {
private:
	SegmentConstraintList constraints_;
public:
	ANDSegmentConstraint(const SegmentConstraintList &constraints);
	virtual bool validSegment(const ChartSegment &segment);
	virtual ~ANDSegmentConstraint();
};

#endif /* ANDSEGMENTCONSTRAINT_H_ */
