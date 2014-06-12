/*
 * SegmentListConstraint.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef SEGMENTLISTCONSTRAINT_H_
#define SEGMENTLISTCONSTRAINT_H_

#include <boost/shared_ptr.hpp>
#include "ChartSegment.h"

class SegmentListConstraint {
public:
	SegmentListConstraint();

	virtual bool validSegments(const ChartSegmentList &segments) = 0;

	virtual ~SegmentListConstraint();
};

typedef boost::shared_ptr<SegmentListConstraint> SegmentListConstraintPtr;

#endif /* SEGMENTLISTCONSTRAINT_H_ */
