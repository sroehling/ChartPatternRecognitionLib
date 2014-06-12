/*
 * ChartSegment.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef CHARTSEGMENT_H_
#define CHARTSEGMENT_H_

#include <boost/shared_ptr.hpp>

#include "PeriodVal.h"
#include "LinearEquation.h"

class ChartSegment {
private:
	PeriodValCltn segmentVals_;
	LinearEquationPtr segmentEq_;
public:
	ChartSegment(const PeriodValCltn &segmentVals);

	const PeriodVal &lastPeriodVal() const;
	const PeriodVal &firstPeriodVal() const;

	virtual ~ChartSegment();
};

typedef boost::shared_ptr<ChartSegment> ChartSegmentPtr;
typedef std::vector<ChartSegmentPtr> ChartSegmentList;

#endif /* CHARTSEGMENT_H_ */
