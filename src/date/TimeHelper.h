/*
 * DateHelper.h
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#ifndef TIMEHELPER_H_
#define TIMEHELPER_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include "DoubleRange.h"

namespace timeHelper {

std::string formatDate(const boost::posix_time::ptime &dateToFormat);

double timeDifferenceMsec(const boost::posix_time::ptime &startTime, const boost::posix_time::ptime &endTime);
bool timeDifferenceValid(const DoubleRange &validTimeDifferences,
                              const boost::posix_time::ptime &startTime, const boost::posix_time::ptime &endTime);

double msecToDays(double msecs);

} // namespace timeHelper

#endif /* DATEHELPER_H_ */
