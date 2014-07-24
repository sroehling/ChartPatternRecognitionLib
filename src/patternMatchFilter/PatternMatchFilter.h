/*
 * PatternMatchFilter.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHFILTER_H_
#define PATTERNMATCHFILTER_H_

#include "PatternMatch.h"
#include "PatternMatchSortFunctor.h"
#include "PatternMatchUniqueFunctor.h"
#include "PatternMatchFindPredicate.h"

namespace patternMatchFilter {

PatternMatchListPtr filterUniqueStartEndTime(const PatternMatchListPtr &unfilteredMatches);

PatternMatchListPtr filterUniqueAndLongestHighestHigh(const PatternMatchListPtr &unfilteredMatches);
PatternMatchListPtr filterUniqueAndLongestLowestLow(const PatternMatchListPtr &unfilteredMatches);


PatternMatchListPtr sortPatternMatches(const PatternMatchListPtr &unsortedMatches,
		const PatternMatchSortFunctor &sortFunc);
PatternMatchListPtr uniquePatternMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchUniqueFunctor &uniqueFunc);

PatternMatchListPtr sortThenFilterUniqueMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchSortFunctor &sortFunc, const PatternMatchUniqueFunctor &uniqueFunc);

// Starting at the beginning of patternMatches, find the first PatternMatch satisfying findPred
PatternMatchList::iterator findFirstPatternMatch(const PatternMatchListPtr &patternMatches,
		const PatternMatchFindPredicate &findPred);

}

#endif /* PATTERNMATCHFILTER_H_ */
