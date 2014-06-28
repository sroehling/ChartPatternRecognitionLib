/*
 * ChartSegment.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "ChartSegment.h"
#include "XYCoord.h"
#include "MathHelper.h"

ChartSegment::ChartSegment(const PeriodValSegmentPtr &segmentVals)
: segmentVals_(segmentVals)
{
	assert(segmentVals->numVals() >= 2);

	// For purposes of creating a LinearEquation based upon the
	// dates and value we need to map the dates onto numerical values.
	double startPtXVal = 0.0;
	double endPtXVal = double(segmentVals->numVals()-1);

	XYCoord startPt(startPtXVal,segmentVals->firstVal().typicalPrice());
	XYCoord endPt(endPtXVal,segmentVals->lastVal().typicalPrice());

	segmentEq_ = LinearEquationPtr(new LinearEquation(startPt,endPt));
}

const PeriodVal &ChartSegment::lastPeriodVal() const
{
	return segmentVals_->lastVal();
}


const PeriodVal &ChartSegment::firstPeriodVal() const
{
	return segmentVals_->firstVal();
}

unsigned int ChartSegment::numPeriods() const
{
	return segmentVals_->numVals();
}


double ChartSegment::slope() const
{
	return segmentEq_->slope();
}


double ChartSegment::maxRelPercentVsLinearEq() const
{
	double maxPerc = -1.0 * std::numeric_limits<double>::max();
	double currXVal = 0.0;
	for(PeriodValCltn::const_iterator valIter = segmentVals_->segBegin();
			valIter != segmentVals_->segEnd(); valIter++)
	{
		double segYVal = (*valIter).typicalPrice();
		double eqYVal = segmentEq_->yVal(currXVal);
		double relPerc = MathHelper::absRelPercentVal(eqYVal,segYVal);
		if(relPerc > maxPerc)
		{
			maxPerc = relPerc;
		}
		currXVal += 1.0;
	}
	return maxPerc;
}

ChartSegment::~ChartSegment() {
	// TODO Auto-generated destructor stub
}


std::ostream& operator<<(std::ostream& os, const ChartSegment& chartSeg)
{
	os << "ChartSegment(slope=" << chartSeg.slope()
			<< ", first val = " << chartSeg.firstPeriodVal()
			<< ", last val = " << chartSeg.lastPeriodVal() << ")";
	return os;
}


