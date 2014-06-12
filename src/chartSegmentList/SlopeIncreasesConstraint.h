/*
 * SlopeIncreasesConstraint.h
 *
 *  Created on: Jun 12, 2014
 *      Author: sroehling
 */

#ifndef SLOPEINCREASESCONSTRAINT_H_
#define SLOPEINCREASESCONSTRAINT_H_

#include "SegmentListConstraint.h"

class SlopeIncreasesConstraint: public SegmentListConstraint {
public:
	SlopeIncreasesConstraint();

	virtual bool validSegments(const ChartSegmentList &segments);

	virtual ~SlopeIncreasesConstraint();
};

#endif /* SLOPEINCREASESCONSTRAINT_H_ */
