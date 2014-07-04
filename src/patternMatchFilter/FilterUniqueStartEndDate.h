/*
 * FilterUniqueStartEndDate.h
 *
 *  Created on: Jul 3, 2014
 *      Author: sroehling
 */

#ifndef FILTERUNIQUESTARTENDDATE_H_
#define FILTERUNIQUESTARTENDDATE_H_

#include "PatternMatch.h"

class FilterUniqueStartEndDate {
public:
	FilterUniqueStartEndDate();

	PatternMatchListPtr filterPatternMatches(const PatternMatchListPtr &unfilteredMatches);

	virtual ~FilterUniqueStartEndDate();
};

#endif /* FILTERUNIQUESTARTENDDATE_H_ */
