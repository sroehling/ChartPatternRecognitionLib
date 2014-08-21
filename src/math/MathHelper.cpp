/*
 * MathHelper.cpp
 *
 *  Created on: Jun 27, 2014
 *      Author: sroehling
 */

#include <MathHelper.h>
#include <limits>
#include <assert.h>

MathHelper::MathHelper() {
}

double MathHelper::maxDouble()
{
	return std::numeric_limits<double>::max();

}

double MathHelper::minDouble()
{
	return -1.0 * std::numeric_limits<double>::max();
}

double MathHelper::relativePercentVal(double comparisonVal, double baseVal)
{
	return (comparisonVal/baseVal - 1.0) * 100.0;
}

double MathHelper::absRelPercentVal(double comparisonVal, double baseVal)
{
	double relPerc = MathHelper::relativePercentVal(comparisonVal,baseVal);
	if(relPerc < 0.0)
	{
		relPerc *= -1.0;
	}
	return relPerc;
}

bool MathHelper::valuesClose(double val1, double val2, double tolerance)
{
    assert(tolerance >= 0.0);
    double delta = abs(val1-val2);
    if(delta <= tolerance)
    {
        return true;
    }
    else
    {
        return false;
    }
}



MathHelper::~MathHelper() {
}

