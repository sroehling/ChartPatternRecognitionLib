/*
 * PeriodVal.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PERIODVAL_H_
#define PERIODVAL_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <list>
#include <ostream>

class PeriodVal {
private:
	boost::posix_time::ptime periodTime_;
	double val_;

public:
	PeriodVal(const boost::posix_time::ptime &periodTime, double val);

	const boost::posix_time::ptime &periodTime() const { return periodTime_; }
	double val() const { return val_; }

	friend std::ostream& operator<<(std::ostream& os, const PeriodVal& perVal);

	virtual ~PeriodVal();
};

typedef std::list<PeriodVal> PeriodValCltn;

#endif /* PERIODVAL_H_ */
