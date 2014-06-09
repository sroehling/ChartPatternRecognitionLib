/*
 * DateValue.h
 *
 *  Created on: Jun 6, 2014
 *      Author: sroehling
 */

#ifndef DATEVALUE_H_
#define DATEVALUE_H_

#include <boost/date_time/gregorian/gregorian.hpp>

class DateValue {
private:
	boost::gregorian::date date_;
	double value_;
public:
	DateValue(const boost::gregorian::date &date, double value);

	double value();
	const boost::gregorian::date &date();

	virtual ~DateValue();
};

#endif /* DATEVALUE_H_ */
