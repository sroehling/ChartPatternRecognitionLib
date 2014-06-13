/*
 * SlopeIncreasesConstraint.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: sroehling
 */

#include <SlopeIncreasesConstraint.h>
#include <limits>

SlopeIncreasesConstraint::SlopeIncreasesConstraint() {
	// TODO Auto-generated constructor stub

}


bool SlopeIncreasesConstraint::validSegments(const ChartSegmentList &segments)
{
	double lastSlope = -1.0 * std::numeric_limits<double>::max();
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

