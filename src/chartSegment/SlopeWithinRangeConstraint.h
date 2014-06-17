/*
 * SlopeWithinRangeConstraint.h
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#ifndef SLOPEWITHINRANGECONSTRAINT_H_
#define SLOPEWITHINRANGECONSTRAINT_H_

#include <SegmentConstraint.h>

class SlopeWithinRangeConstraint: public SegmentConstraint {
private:
	double minSlope_;
	double maxSlope_;
public:
	SlopeWithinRangeConstraint(double minSlope, double maxSlope);

	bool validSegment(const ChartSegment &segment);

	virtual ~SlopeWithinRangeConstraint();
};

#endif /* SLOPEWITHINRANGECONSTRAINT_H_ */
