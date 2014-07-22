/*
 * SegmentValsCloseToLinearEq.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "SegmentValsCloseToLinearEq.h"

SegmentValsCloseToLinearEq::SegmentValsCloseToLinearEq(double maxDeltaPercVsEquation)
: maxDeltaPercVsEquation_(maxDeltaPercVsEquation)
{
	assert(maxDeltaPercVsEquation > 0.0);
}

bool SegmentValsCloseToLinearEq::validSegment(const ChartSegment &segment)
{
	if(segment.maxRelPercentVsLinearEq() <= maxDeltaPercVsEquation_)
	{
		return true;
	}
	else
	{
		return false;
	}
}

SegmentValsCloseToLinearEq::~SegmentValsCloseToLinearEq() {
\}

