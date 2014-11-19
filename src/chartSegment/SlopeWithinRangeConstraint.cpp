/*
 * SlopeWithinRangeConstraint.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <SlopeWithinRangeConstraint.h>

SlopeWithinRangeConstraint::SlopeWithinRangeConstraint(const DoubleRange &slopeRange)
: slopeRange_(slopeRange)
{
}

bool SlopeWithinRangeConstraint::validSegment(const ChartSegment &segment)
{
	if((segment.slope() >= slopeRange_.minVal()) && (segment.slope() <= slopeRange_.maxVal()))
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

