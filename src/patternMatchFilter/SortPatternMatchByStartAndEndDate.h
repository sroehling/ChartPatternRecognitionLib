/*
 * SorPatternMatchByStartAndEndDate.h
 *
 *  Created on: Jul 3, 2014
 *      Author: sroehling
 */

#ifndef SORTPATTERNMATCHBYSTARTANDENDDATE_H_
#define SORTPATTERNMATCHBYSTARTANDENDDATE_H_

#include "PatternMatch.h"

class SortPatternMatchByStartAndEndDate {
public:
	bool operator() (const PatternMatchPtr &first, const PatternMatchPtr &second) const;
};

#endif /* SORTPATTERNMATCHBYSTARTANDENDDATE_H_ */
