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

