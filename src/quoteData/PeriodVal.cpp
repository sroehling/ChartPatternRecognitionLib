/*
 * PeriodVal.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PeriodVal.h"

PeriodVal::PeriodVal(const boost::posix_time::ptime &periodTime, double val)
: periodTime_(periodTime), val_(val)
{

}

PeriodVal::~PeriodVal() {
	// TODO Auto-generated destructor stub
}

std::ostream& operator<<(std::ostream& os, const PeriodVal& perVal)
{
	os << "PeriodVal(" << perVal.periodTime_ << ", " << perVal.val_ << ")";
	return os;
}
