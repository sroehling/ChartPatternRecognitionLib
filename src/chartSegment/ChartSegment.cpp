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

double ChartSegment::slope() const
{
	return segmentEq_->slope();
}

double ChartSegment::absRelPercentVal(double comparisonVal, double baseVal) const
{
	double relPerc = (comparisonVal/baseVal - 1.0) * 100.0;
	if(relPerc < 0.0)
	{
		relPerc *= -1.0;
	}
	return relPerc;
}

double ChartSegment::maxRelPercentVsLinearEq() const
{
	double maxPerc = -1.0 * std::numeric_limits<double>::max();
	double currXVal = 0.0;
	for(PeriodValCltn::const_iterator valIter = segmentVals_.begin();
			valIter != segmentVals_.end(); valIter++)
	{
		double segYVal = (*valIter).val();
		double eqYVal = segmentEq_->yVal(currXVal);
		double relPerc = absRelPercentVal(eqYVal,segYVal);
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


