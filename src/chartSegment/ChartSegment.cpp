/*
 * ChartSegment.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "ChartSegment.h"
#include "XYCoord.h"
#include "MathHelper.h"
#include "PeriodValueRef.h"

ChartSegment::ChartSegment(const PeriodValSegmentPtr &segmentVals)
: segmentVals_(segmentVals)
{
	assert(segmentVals->numVals() >= 2);
	segmentEq_ = segmentVals->segmentEquation(TypicalPricePeriodValueRef());
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
	for(PeriodValCltn::const_iterator valIter = segmentVals_->segBegin();
			valIter != segmentVals_->segEnd(); valIter++)
	{
		double segYVal = (*valIter).typicalPrice();
		double eqYVal = segmentEq_->yVal((*valIter).pseudoXVal());
		double relPerc = MathHelper::absRelPercentVal(eqYVal,segYVal);
		if(relPerc > maxPerc)
		{
			maxPerc = relPerc;
		}
	}
	return maxPerc;
}

ChartSegment::~ChartSegment() {
}


std::ostream& operator<<(std::ostream& os, const ChartSegment& chartSeg)
{
	os << "ChartSegment(slope=" << chartSeg.slope()
			<< ", first val = " << chartSeg.firstPeriodVal()
			<< ", last val = " << chartSeg.lastPeriodVal() << ")";
	return os;
}


