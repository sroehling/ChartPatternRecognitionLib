/*
 * PatternMatchSortFunctor.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <PatternMatchSortFunctor.h>

bool SortPatternMatchByEndTimeThenLength::operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const
{
	// First sort by endTime()
	if(first->endTime() < second->endTime())
	{
		return true;
	}
	else if(first->endTime() == second->endTime())
	{
		// Then sort by startTime(), earliest startTime() first
		if(first->startTime() < second->startTime())
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

bool SortPatternMatchByStartAndEndDate::operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const
{

	if(first->startTime() < second->startTime())
	{
		return true;
	}
	else if(first->startTime() == second->startTime())
	{
		if(first->endTime() < second->endTime())
		{
			return true;
		}
		else if(first->endTime() == second->endTime())
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

