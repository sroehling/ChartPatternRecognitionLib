/*
 * PatternMatchFindPredicate.h
 *
 *  Created on: Jul 23, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHFINDPREDICATE_H_
#define PATTERNMATCHFINDPREDICATE_H_

#include "PatternMatch.h"
#include <boost/date_time/posix_time/posix_time.hpp>


class PatternMatchFindPredicate {
public:
	PatternMatchFindPredicate() {}

	virtual bool operator() (const PatternMatchPtr &patternMatch) const = 0;

	virtual ~PatternMatchFindPredicate() {}
};

class LowestLowAfterTime: public PatternMatchFindPredicate {
private:
	boost::posix_time::ptime comparisonTime_;
public:
	LowestLowAfterTime(const boost::posix_time::ptime &comparisonTime) : comparisonTime_(comparisonTime) {}

	virtual bool operator() (const PatternMatchPtr &patternMatch) const;

	virtual ~LowestLowAfterTime() {}
};


#endif /* PATTERNMATCHFINDPREDICATE_H_ */
