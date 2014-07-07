/*
 * PatternMatchValueRef.h
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVALUEREF_H_
#define PATTERNMATCHVALUEREF_H_

#include <boost/shared_ptr.hpp>
#include "PatternMatch.h"

class PatternMatchValueRef {
public:
	PatternMatchValueRef() {}
	virtual double patternMatchVal(const PatternMatch &match) const = 0;
	virtual ~PatternMatchValueRef() {}
};

typedef boost::shared_ptr<PatternMatchValueRef> PatternMatchValueRefPtr;

class LowestLowPatternMatchValueRef : public PatternMatchValueRef {
public:
	virtual double patternMatchVal(const PatternMatch &match) const;
};

class FixedPatternMatchValueRef : public PatternMatchValueRef {
private:
	double val_;
public:
	FixedPatternMatchValueRef(double val) : val_(val) {}
	virtual double patternMatchVal(const PatternMatch &match) const;
};


#endif /* PATTERNMATCHVALUEREF_H_ */
