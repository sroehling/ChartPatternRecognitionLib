/*
 * UnsignedIntRange.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: sroehling
 */

#include <UnsignedIntRange.h>
#include <assert.h>

UnsignedIntRange::UnsignedIntRange(unsigned int minVal, unsigned int maxVal)
: minVal_(minVal), maxVal_(maxVal)
{
	assert(maxVal>=minVal);
}
