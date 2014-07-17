/*
 * PatternMatchUniqueFunctor.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <PatternMatchUniqueFunctor.h>

bool SameStartAndEndDate::operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const
{
	  return( (first->startTime() == second->startTime()) &&
			 (first->endTime() == second->endTime())  );
}

bool SameEndTime::operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const
{
	  return(first->endTime() == second->endTime() );
}
