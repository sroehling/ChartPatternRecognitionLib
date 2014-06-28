/*
 * EndWithinPercentOfStart.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "EndWithinPercentOfStart.h"
#include "MathHelper.h"

EndWithinPercentOfStart::EndWithinPercentOfStart(double relPercent)
: relPercent_(relPercent)
{
	assert(relPercent >= -100.0);
	assert(relPercent != 0.0);
}


bool EndWithinPercentOfStart::validPattern(const PatternMatch &candidateMatch)
{
	double startingVal = candidateMatch.firstValue().high();
	double endingVal = candidateMatch.lastValue().close();
	double relPercResult = MathHelper::relativePercentVal(endingVal,startingVal);

//	std::cerr << "EndWithinPercentOfStart: Relative Percent Val: start=" << startingVal << " end=" << endingVal
//			<< " relPerc=" << relPercResult << std::endl;

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
}

