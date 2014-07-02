/*
 * DateHelper.h
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#ifndef TIMEHELPER_H_
#define TIMEHELPER_H_

#include <boost/date_time/posix_time/posix_time.hpp>


class TimeHelper {
public:
	TimeHelper();

	static std::string formatDate(const boost::posix_time::ptime &dateToFormat);

	virtual ~TimeHelper();
};

#endif /* DATEHELPER_H_ */
