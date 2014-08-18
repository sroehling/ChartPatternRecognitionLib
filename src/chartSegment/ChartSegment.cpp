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
	perValRef_ = PeriodValueRefPtr(new TypicalPricePeriodValueRef());
	segmentEq_ = segmentVals->segmentEquation(*perValRef_);
}

ChartSegment::ChartSegment(const PeriodValCltnPtr &perValCltn,
		const PeriodValCltn::iterator &startPt, const PeriodValCltn::iterator &endPt,
			const PeriodValueRefPtr &endPtValueRef)
{
	perValRef_ = endPtValueRef;

	// PeriodValSegment works like a regular STL iterator, in that iteration over
	// the segment includes everything *except* segEnd. So, to include the value
	// pointed to by endPatternMatchIter, we need iterate one beyond endPatternMatchIter.
	PeriodValCltn::iterator segEnd = endPt;
	assert(endPt != perValCltn->end());
	segEnd++;

	segmentVals_ = PeriodValSegmentPtr(new PeriodValSegment(perValCltn,startPt,segEnd));
	segmentEq_ = segmentVals_->segmentEquation(*perValRef_);
	assert(segmentVals_->numVals() >= 2);
}

const PeriodVal &ChartSegment::lastPeriodVal() const
{
	return segmentVals_->lastVal();
}


const PeriodVal &ChartSegment::firstPeriodVal() const
{
	return segmentVals_->firstVal();
}

const PeriodValCltn::iterator ChartSegment::lastValIter() const
{
	return segmentVals_->lastValIter();
}

const PeriodValCltn::iterator &ChartSegment::firstValIter() const
{
	return segmentVals_->firstValIter();
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
	// TODO - Should this iteration include segEnd()? This appears to be an off-by-one bug.
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


