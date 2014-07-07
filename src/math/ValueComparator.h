/*
 * ValueComparator.h
 *
 *  Created on: Jul 7, 2014
 *      Author: sroehling
 */

#ifndef VALUECOMPARATOR_H_
#define VALUECOMPARATOR_H_

#include <boost/shared_ptr.hpp>


class ValueComparator {
public:
	ValueComparator() {}

	virtual bool compare(double lhs, double rhs) const = 0;

	virtual ~ValueComparator() {}
};

typedef boost::shared_ptr<ValueComparator> ValueComparatorPtr;

class EqualValueComparator: public ValueComparator {
	bool compare(double lhs, double rhs) const { return lhs==rhs; }
};

class LessThanValueComparator: public ValueComparator {
	bool compare(double lhs, double rhs) const { return lhs<rhs; }
};

class LessThanEqualValueComparator: public ValueComparator {
	bool compare(double lhs, double rhs) const { return lhs<=rhs; }
};


class GreaterThanValueComparator: public ValueComparator {
	bool compare(double lhs, double rhs) const { return lhs>rhs; }
};

class GreaterThanEqualValueComparator: public ValueComparator {
	bool compare(double lhs, double rhs) const { return lhs>=rhs; }
};


#endif /* VALUECOMPARATOR_H_ */
