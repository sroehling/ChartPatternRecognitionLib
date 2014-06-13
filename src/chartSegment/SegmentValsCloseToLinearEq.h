/*
 * SegmentValsCloseToLinearEq.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef SEGMENTVALSCLOSETOLINEAREQ_H_
#define SEGMENTVALSCLOSETOLINEAREQ_H_

#include "SegmentConstraint.h"

class SegmentValsCloseToLinearEq: public SegmentConstraint {
private:
	double maxDeltaPercVsEquation_;
public:
	SegmentValsCloseToLinearEq(double maxDeltaPercVsEquation);

	bool validSegment(const ChartSegment &segment);

	virtual ~SegmentValsCloseToLinearEq();
};

#endif /* SEGMENTVALSCLOSETOLINEAREQ_H_ */
