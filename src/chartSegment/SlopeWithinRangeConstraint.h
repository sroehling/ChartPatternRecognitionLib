/*
 * SlopeWithinRangeConstraint.h
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#ifndef SLOPEWITHINRANGECONSTRAINT_H_
#define SLOPEWITHINRANGECONSTRAINT_H_

#include <SegmentConstraint.h>
#include "DoubleRange.h"

class SlopeWithinRangeConstraint: public SegmentConstraint {
private:
	DoubleRange slopeRange_;
public:
	SlopeWithinRangeConstraint(const DoubleRange &slopeRange);

	bool validSegment(const ChartSegment &segment);

	virtual ~SlopeWithinRangeConstraint();
};

#endif /* SLOPEWITHINRANGECONSTRAINT_H_ */
