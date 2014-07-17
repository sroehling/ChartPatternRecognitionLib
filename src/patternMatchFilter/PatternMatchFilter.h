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

namespace patternMatchFilter {

PatternMatchListPtr filterUniqueStartEndTime(const PatternMatchListPtr &unfilteredMatches);

PatternMatchListPtr sortPatternMatches(const PatternMatchListPtr &unsortedMatches,
		const PatternMatchSortFunctor &sortFunc);
PatternMatchListPtr uniquePatternMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchUniqueFunctor &uniqueFunc);

PatternMatchListPtr sortThenFilterUniqueMatches(const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchSortFunctor &sortFunc, const PatternMatchUniqueFunctor &uniqueFunc);

}

#endif /* PATTERNMATCHFILTER_H_ */
