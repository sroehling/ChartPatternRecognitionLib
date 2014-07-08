/*
 * TestPerValRange.h
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#ifndef TESTPERVALRANGE_H_
#define TESTPERVALRANGE_H_

#include <list>

class TestPerValRange {
private:
	unsigned int numVals_;
	double startVal_;
	double endVal_;

public:
	TestPerValRange(unsigned int numVals, double startVal, double endVal);

	double startVal() const { return startVal_; }
	double endVal() const { return endVal_; }
	unsigned int numVals() const { return numVals_; }
	double valIncr() const;

	virtual ~TestPerValRange();
};

#endif /* TESTPERVALRANGE_H_ */

typedef std::list<TestPerValRange> TestPerValRangeList;
