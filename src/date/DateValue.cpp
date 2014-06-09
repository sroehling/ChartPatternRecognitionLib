/*
 * DateValue.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#include "DateValue.h"

DateValue::DateValue(const boost::gregorian::date &date, double value)
: date_(date), value_(value)
{
	// TODO Auto-generated constructor stub

}

double DateValue::value()
{
	return value_;
}

 const boost::gregorian::date &DateValue::date()
{
	return date_;
}


DateValue::~DateValue() {
	// TODO Auto-generated destructor stub
}

