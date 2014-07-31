/*
 * UnsignedIntRange.h
 *
 *  Created on: Jul 31, 2014
 *      Author: sroehling
 */

#ifndef UNSIGNEDINTRANGE_H_
#define UNSIGNEDINTRANGE_H_

class UnsignedIntRange {
private:
	unsigned int minVal_;
	unsigned int maxVal_;
public:
	UnsignedIntRange(unsigned int minVal, unsigned int maxVal);

	bool valueWithinRange(unsigned int val) const { return (val >= minVal_) && (val <= maxVal_); }

	unsigned int maxVal() const { return maxVal_; }
	unsigned int minVal() const { return minVal_; }

	virtual ~UnsignedIntRange() {}
};

#endif /* UNSIGNEDINTRANGE_H_ */
