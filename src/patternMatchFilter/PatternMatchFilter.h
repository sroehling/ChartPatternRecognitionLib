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

// Filter then return unique matches with the same lowest-low or highest-high. This is useful
// for filtering the pivot lows or pivot highs.
PatternMatchListPtr filterUniqueAndLongestHighestHigh(const PatternMatchListPtr &unfilteredMatches);
PatternMatchListPtr filterUniqueAndLongestLowestLow(const PatternMatchListPtr &unfilteredMatches);

// Sort the patterns by their end date, then by length (from longest to shortest). Then
// return a unique list of patterns with the same end date.
PatternMatchListPtr filterUniqueLongestPatternSameEndDate(const PatternMatchListPtr &unfilteredMatches);

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
