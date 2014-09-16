/*
 * PatternMatchSortFunctor.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHSORTFUNCTOR_H_
#define PATTERNMATCHSORTFUNCTOR_H_

#include "PatternMatch.h"

class PatternMatchSortFunctor {
public:
	PatternMatchSortFunctor() {}

	virtual bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const = 0;

	virtual ~PatternMatchSortFunctor() {}
};

// Sort by end date, then by start date. This will give you a PatternMatchList where
// PatternMatch objects with the same end date are adjacent to one another. Then
// PatternMatch objects with the same end date are sorted by start date with the
// PatternMatch object with earlier start time coming first.
//
// The result of this sort is suitable for use with the PatternMatchList unique method.
// Combined with the unique function, the results will be PatternMatch objects with unique end
// dates, but with the earliest start dates for the same end dates (in other words, longest
// PatternMatch leading up to the end date).
class SortPatternMatchByEndTimeThenLength : public PatternMatchSortFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};

class ReverseSortPatternMatchByEndTimeThenLength : public PatternMatchSortFunctor {
public:
    bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};


// Sort first by start date, then by end date, from earliest to latest dates for each
class SortPatternMatchByStartAndEndDate  : public PatternMatchSortFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};

// The PatternMatch class has a highestHighTime member function. This method
// returns the period time when the highest high took place. This is useful
// to first sort by the highest high, then the length of the pattern to identify
// unique pivot points with the same highest high.
class SortPatternMatchByHighestHighTimeThenLength : public PatternMatchSortFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};

class SortPatternMatchByLowestLowTimeThenLength : public PatternMatchSortFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};



#endif /* PATTERNMATCHSORTFUNCTOR_H_ */
