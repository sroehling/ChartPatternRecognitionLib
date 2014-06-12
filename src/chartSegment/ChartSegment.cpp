/*
 * ChartSegment.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "ChartSegment.h"
#include "XYCoord.h"

ChartSegment::ChartSegment(const PeriodValCltn &segmentVals)
: segmentVals_(segmentVals)
{
	assert(segmentVals.size() >= 2);

	// For purposes of creating a LinearEquation based upon the
	// dates and value we need to map the dates onto numerical values.
	double startPtXVal = 0.0;
	double endPtXVal = double(segmentVals_.size()-1);

	XYCoord startPt(startPtXVal,segmentVals_.front().val());
	XYCoord endPt(endPtXVal,segmentVals_.back().val());

	segmentEq_ = LinearEquationPtr(new LinearEquation(startPt,endPt));
}

const PeriodVal &ChartSegment::lastPeriodVal() const
{
	return segmentVals_.back();
}


const PeriodVal &ChartSegment::firstPeriodVal() const
{
	return segmentVals_.front();
}


ChartSegment::~ChartSegment() {
	// TODO Auto-generated destructor stub
}

