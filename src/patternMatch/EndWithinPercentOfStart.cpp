/*
 * EndWithinPercentOfStart.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "EndWithinPercentOfStart.h"

EndWithinPercentOfStart::EndWithinPercentOfStart(double relPercent)
: relPercent_(relPercent)
{
	assert(relPercent >= -100.0);
	assert(relPercent != 0.0);
}

double EndWithinPercentOfStart::relativePercentVal(double comparisonVal, double baseVal) const
{
	return (comparisonVal/baseVal - 1.0) * 100.0;
}


bool EndWithinPercentOfStart::validPattern(const PatternMatch &candidateMatch)
{
	double startingVal = candidateMatch.firstValue().val();
	double endingVal = candidateMatch.lastValue().val();
	double relPercResult = relativePercentVal(endingVal,startingVal);

	if(relPercent_ < 0.0)
	{
		if((relPercResult < 0.0) && (relPercResult >= relPercent_))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else // relPercent_ > 0.0
	{
		if((relPercResult > 0.0) && (relPercResult <= relPercent_))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}


EndWithinPercentOfStart::~EndWithinPercentOfStart() {
	// TODO Auto-generated destructor stub
}

