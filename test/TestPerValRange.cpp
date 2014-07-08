/*
 * TestPerValRange.cpp
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#include <TestPerValRange.h>
#include <assert.h>

TestPerValRange::TestPerValRange(unsigned int numVals, double startVal, double endVal)
: numVals_(numVals),
  startVal_(startVal),
  endVal_(endVal)
{
	assert(numVals >= 2);
	assert(startVal != endVal);
}

double TestPerValRange::valIncr() const
{
	return (endVal() - startVal())/(double)(numVals()-1);
}



TestPerValRange::~TestPerValRange() {
}

