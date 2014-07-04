/*
 * SorPatternMatchByStartAndEndDate.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: sroehling
 */

#include <SortPatternMatchByStartAndEndDate.h>


bool SortPatternMatchByStartAndEndDate::operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const
{

	if(first->firstValue().periodTime() < second->firstValue().periodTime())
	{
		return true;
	}
	else if(first->firstValue().periodTime() == second->firstValue().periodTime())
	{
		if(first->lastValue().periodTime() < second->lastValue().periodTime())
		{
			return true;
		}
		else if(first->lastValue().periodTime() == second->lastValue().periodTime())
		{
			// As a 3rd sort key, order from least number segments to the most.
			// When a list of matches is first sorted by this function, then unique is applied,
			// this will result in a set of matches with the fewest number of segments.
			if(first->numSegments() < second->numSegments())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


