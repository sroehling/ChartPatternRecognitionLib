/*
 * DateHelper.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include "TimeHelper.h"
#include <sstream>

using namespace boost::posix_time;

namespace timeHelper {

std::string formatDate(const boost::posix_time::ptime &dateToFormat)
{
  using namespace boost::posix_time;


  time_facet *facet = new time_facet("%Y-%m-%d");
  std::ostringstream oss;
  oss.imbue(std::locale(oss.getloc(), facet));

  oss << dateToFormat;

  return oss.str();
}

double timeDifferenceMsec(const ptime &startTime, const ptime &endTime)
{
    assert(endTime>startTime);
    time_duration startEndPivotTimeDiff = endTime - startTime;
    double startEndPivotTimeDiffMsec = (double)startEndPivotTimeDiff.total_milliseconds();
    assert(startEndPivotTimeDiffMsec > 0.0);
    return startEndPivotTimeDiffMsec;
}

bool timeDifferenceValid(const DoubleRange &validTimeDifferences,
                              const ptime &startTime, const ptime &endTime)
{
    if(validTimeDifferences.valueWithinRange(timeDifferenceMsec(startTime,endTime)))
    {
        return true;
    }
    else
    {
        return false;
    }

}


}

