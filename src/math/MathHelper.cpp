/*
 * MathHelper.cpp
 *
 *  Created on: Jun 27, 2014
 *      Author: sroehling
 */

#include <MathHelper.h>
#include <limits>


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



MathHelper::~MathHelper() {
}

