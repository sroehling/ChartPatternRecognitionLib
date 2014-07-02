/*
 * SlopeWithinRangeConstraint.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
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
		/*
		BOOST_LOG_TRIVIAL(debug) <<
				"SlopeWithinRangeConstraint: ChartSegment failed slope constraint (min,max) = ("
				  << minSlope_ << "," << maxSlope_ << ") segment="
				  << segment;
				  */
		return false;
	}
}

SlopeWithinRangeConstraint::~SlopeWithinRangeConstraint() {
}

