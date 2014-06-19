/*
 * AnySegmentListValidConstraint.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#ifndef ANYSEGMENTLISTVALIDCONSTRAINT_H_
#define ANYSEGMENTLISTVALIDCONSTRAINT_H_

#include <SegmentListConstraint.h>

// SegmentListConstraint for testing - validSegments() always
// returns true.
class AnySegmentListValidConstraint: public SegmentListConstraint {
public:
	AnySegmentListValidConstraint();
	virtual bool validSegments(const ChartSegmentList &segments);
	virtual ~AnySegmentListValidConstraint();
};

#endif /* ANYSEGMENTLISTVALIDCONSTRAINT_H_ */
