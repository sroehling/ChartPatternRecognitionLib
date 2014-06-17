/*
 * SlopeWithinRangeConstraint.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <SlopeWithinRangeConstraint.h>

SlopeWithinRangeConstraint::SlopeWithinRangeConstraint(double minSlope, double maxSlope)
: minSlope_(minSlope),
  maxSlope_(maxSlope)
{
	assert(maxSlope >= minSlope);

}

bool SlopeWithinRangeConstraint::validSegment(const ChartSegment &segment)
{
	if((segment.slope() >= minSlope_) && (segment.slope() <= maxSlope_))
	{
		return true;
	}
	else
	{
		return false;
	}
}

SlopeWithinRangeConstraint::~SlopeWithinRangeConstraint() {
}

