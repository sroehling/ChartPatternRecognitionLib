/*
 * SlopeIncreasesConstraint.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: sroehling
 */

#include <SlopeIncreasesConstraint.h>
#include "MathHelper.h"

SlopeIncreasesConstraint::SlopeIncreasesConstraint() {
}


bool SlopeIncreasesConstraint::validSegments(const ChartSegmentList &segments)
{
	double lastSlope = MathHelper::minDouble();
	for(ChartSegmentList::const_iterator segIter = segments.begin();
			segIter != segments.end(); segIter++)
	{
		double currSlope = (*segIter)->slope();
		if(currSlope < lastSlope)
		{
			return false;
		}
		lastSlope = currSlope;
	}
	return true;
}


SlopeIncreasesConstraint::~SlopeIncreasesConstraint() {
	// TODO Auto-generated destructor stub
}

