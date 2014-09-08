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
#include "PeriodValueRef.h"

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

class HighestHighPatternMatchValueRef : public PatternMatchValueRef {
public:
    virtual double patternMatchVal(const PatternMatch &match) const;
};


class HighestValPatternMatchValueRef : public PatternMatchValueRef {
private:
    PeriodValueRefPtr valueRef_;
public:
    HighestValPatternMatchValueRef(const PeriodValueRefPtr &valueRef);
    virtual double patternMatchVal(const PatternMatch &match) const;
};


class DepthPercentPatternMatchValueRef : public PatternMatchValueRef {
public:
	virtual double patternMatchVal(const PatternMatch &match) const;
};

class DepthPointsPatternMatchValueRef : public PatternMatchValueRef {
public:
    virtual double patternMatchVal(const PatternMatch &match) const;
};


class DepthPointsFirstHighLowestLowPatternMatchValueRef : public PatternMatchValueRef {
public:
    virtual double patternMatchVal(const PatternMatch &match) const;
};


class NumPeriodsPatternMatchValueRef : public PatternMatchValueRef {
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

class LastPeriodValPatternMatchValueRef : public PatternMatchValueRef {
private:
	PeriodValueRefPtr valRefWithinLastPeriodVal_;
public:
	LastPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinLastPeriodVal);
	virtual double patternMatchVal(const PatternMatch &match) const;
};

class SecondToLastPeriodValPatternMatchValueRef : public PatternMatchValueRef {
private:
    PeriodValueRefPtr valRefWithinPeriodVal_;
public:
    SecondToLastPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinPeriodVal);
    virtual double patternMatchVal(const PatternMatch &match) const;
};


class FirstPeriodValPatternMatchValueRef : public PatternMatchValueRef {
private:
	PeriodValueRefPtr valRefWithinPeriodVal_;
public:
	FirstPeriodValPatternMatchValueRef(const PeriodValueRefPtr &valRefWithinPeriodVal);
	virtual double patternMatchVal(const PatternMatch &match) const;
};



#endif /* PATTERNMATCHVALUEREF_H_ */
