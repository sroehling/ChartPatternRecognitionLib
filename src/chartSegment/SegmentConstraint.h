/*
 * SegmentConstraint.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef SEGMENTCONSTRAINT_H_
#define SEGMENTCONSTRAINT_H_

#include <boost/shared_ptr.hpp>
#include "ChartSegment.h"

class SegmentConstraint {
public:
	virtual bool validSegment(const ChartSegment &segment) = 0;
	virtual ~SegmentConstraint() {}
};

typedef boost::shared_ptr<SegmentConstraint> SegmentConstraintPtr;
typedef std::list<SegmentConstraintPtr> SegmentConstraintList;

#endif /* SEGMENTCONSTRAINT_H_ */
