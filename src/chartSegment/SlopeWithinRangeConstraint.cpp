/*
 * SlopeWithinRangeConstraint.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>
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

