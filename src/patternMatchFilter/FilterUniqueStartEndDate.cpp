/*
 * FilterUniqueStartEndDate.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: sroehling
 */

#include "FilterUniqueStartEndDate.h"
#include "SortPatternMatchByStartAndEndDate.h"
#include <boost/iterator/indirect_iterator.hpp>
#include <algorithm>


struct SameStartAndEndDate {
  bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second)
  {
	  return( (first->firstValue().periodTime() == second->firstValue().periodTime()) &&
			 (first->lastValue().periodTime() == second->lastValue().periodTime())  );
  }
};


FilterUniqueStartEndDate::FilterUniqueStartEndDate() {
}

PatternMatchListPtr FilterUniqueStartEndDate::filterPatternMatches(const PatternMatchListPtr &unfilteredMatches)
{
	// Copy the original list
	PatternMatchListPtr filteredList(new PatternMatchList);
	filteredList->insert(filteredList->end(),unfilteredMatches->begin(), unfilteredMatches->end());

	SortPatternMatchByStartAndEndDate sortFunc;

	filteredList->sort(sortFunc);

	SameStartAndEndDate uniquePred;
	filteredList->unique(uniquePred);

	return filteredList;
}


FilterUniqueStartEndDate::~FilterUniqueStartEndDate() {
}

