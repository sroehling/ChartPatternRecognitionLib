/*
 * PeriodData.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PERIODDATA_H_
#define PERIODDATA_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <list>


class PeriodData;
typedef std::list<PeriodData> PeriodDataCltn;
typedef boost::shared_ptr<PeriodDataCltn> PeriodDataCltnPtr;


class PeriodData {

private:
	boost::posix_time::ptime periodTime_;
	double open_;
	double high_;
	double low_;
	double close_;
	unsigned int volume_;


public:
	PeriodData(boost::posix_time::ptime &periodTime,
			double open, double high, double low, double close, unsigned int volume);

	const boost::posix_time::ptime &periodTime() const { return periodTime_; }
	double open() { return open_; }
	double high() { return high_; }
	double low() { return low_; }
	double close() { return close_; }
	unsigned int volume() { return volume_; }

	static PeriodDataCltnPtr readFromFile(const std::string &fileName);

	friend std::ostream& operator<<(std::ostream& os, const PeriodData& perData);


	virtual ~PeriodData();
};

#endif /* PERIODDATA_H_ */
