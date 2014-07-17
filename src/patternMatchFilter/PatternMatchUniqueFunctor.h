/*
 * PatternMatchUniqueFunctor.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHUNIQUEFUNCTOR_H_
#define PATTERNMATCHUNIQUEFUNCTOR_H_

#include "PatternMatch.h"


class PatternMatchUniqueFunctor {
public:
	PatternMatchUniqueFunctor() {}

	virtual bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const =0;

	virtual ~PatternMatchUniqueFunctor() {}
};

class SameStartAndEndDate: public PatternMatchUniqueFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};

class SameEndTime: public PatternMatchUniqueFunctor {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};


#endif /* PATTERNMATCHUNIQUEFUNCTOR_H_ */
