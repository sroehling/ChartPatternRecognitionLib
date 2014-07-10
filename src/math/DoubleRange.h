/*
 * DoubleRange.h
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#ifndef DOUBLERANGE_H_
#define DOUBLERANGE_H_

class DoubleRange {
private:
	double minVal_;
	double maxVal_;
public:
	DoubleRange(double minVal, double maxVal);

	double minVal() const { return minVal_; }
	double maxVal() const { return maxVal_; }

	bool positiveVals() const { return (minVal() >= 0.0) && (maxVal() >=0.0); }
	bool valueWithinRange(double val) const { return (val >= minVal()) && (val <= maxVal()); }

	virtual ~DoubleRange() {}
};

#endif /* DOUBLERANGE_H_ */
