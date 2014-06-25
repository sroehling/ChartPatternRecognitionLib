/*
 * PeriodVal.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PERIODVAL_H_
#define PERIODVAL_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <ostream>

class PeriodVal;
typedef std::list<PeriodVal> PeriodValCltn;
typedef boost::shared_ptr<PeriodValCltn> PeriodValCltnPtr;


class PeriodVal {
private:
	boost::posix_time::ptime periodTime_;
	double open_;
	double high_;
	double low_;
	double close_;
	unsigned int volume_;

public:
	PeriodVal(boost::posix_time::ptime &periodTime,
			double open, double high, double low, double close, unsigned int volume);

	const boost::posix_time::ptime &periodTime() const { return periodTime_; }
	double open() const { return open_; }
	double high() const { return high_; }
	double low() const { return low_; }
	double close() const { return close_; }
	unsigned int volume() const { return volume_; }

	static PeriodValCltnPtr readFromFile(const std::string &fileName);

	double typicalPrice() const;

	friend std::ostream& operator<<(std::ostream& os, const PeriodVal& perVal);

	virtual ~PeriodVal();
};



#endif /* PERIODVAL_H_ */
