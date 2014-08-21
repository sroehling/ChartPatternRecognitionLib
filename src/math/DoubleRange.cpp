/*
 * DoubleRange.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#include <DoubleRange.h>
#include <assert.h>

DoubleRange::DoubleRange(double minVal, double maxVal)
: minVal_(minVal),
  maxVal_(maxVal)
{
	assert(maxVal >= minVal);
}

double DoubleRange::percentWithinRange(double val) const
{
	assert(val >= minVal_);
	assert(val <= maxVal_);

	if(maxVal_==minVal_) { return 1.0; }

	double range = maxVal_-minVal_;

	return (val-minVal_)/range;
}

double DoubleRange::percentMidpointVal(double percMid) const
{
    assert(percMid >=0.0);
    assert(percMid <= 1.0);

    double deltaVal = maxVal_-minVal_;

    return minVal_ + percMid * deltaVal;
}

std::ostream& operator<<(std::ostream& os, const DoubleRange& range)
{
	os << "DoubleRange("
			<< "min=" << range.minVal_
			<< ",max=" << range.maxVal_
			<< ")";
	return os;
}
