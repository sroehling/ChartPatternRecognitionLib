/*
 * DateHelper.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include "TimeHelper.h"
#include <sstream>
#include <boost/format.hpp>
//#include <boost/algorithm/string/join.hpp>
//#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::posix_time;

#define TIME_HELPER_MSEC_PER_SEC 1000.0
#define TIME_HELPER_SEC_PER_DAY 86400.0

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

boost::posix_time::ptime parseDateFromString(const std::string &dateStr)
{
    namespace bt = boost::posix_time;

    // The following is a work-around for limitations of Boost date parsing. In particular,
    // the %d format requires a leading zero. We accommodate this by padding the date string with
    // a leading zero if the first part of the date only has 1 character.
    std::vector<std::string> dateParts;
    boost::split(dateParts, dateStr, boost::is_any_of("-"));
    if(dateParts.size() != 3)
    {
        std::string errorMsg = boost::str(boost::format("Invalid date format: [%s]")%dateStr);
        std::cerr << "parseDateFromString: " << errorMsg << std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
    }
    if(dateParts[0].size() ==1)
    {
        std::string firstDatePartWithLeadingZero("0");
        firstDatePartWithLeadingZero.append(dateParts[0]);
        dateParts[0] = firstDatePartWithLeadingZero;
    }
    std::string dateStrWithLeadingZero = boost::algorithm::join(dateParts, "-");

    const std::locale formats[] = {
        std::locale(std::locale::classic(),new bt::time_input_facet("%d-%b-%y")), // e.g.: 11-Nov-12, 07-Nov-12 (note the leading zero)
        std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d")) // e.g. 2012-11-12
    };

    const size_t numFormats = sizeof(formats)/sizeof(formats[0]);

    bt::ptime parsedTime;
    for(size_t formatIndex=0; formatIndex<numFormats; formatIndex++)
    {
        std::istringstream is(dateStrWithLeadingZero);
        is.imbue(formats[formatIndex]);
        is >> parsedTime;
        if(parsedTime != bt::ptime())
        {
            return parsedTime;
        }
    }
    std::string errorMsg = boost::str(boost::format("Invalid date format: [%s]")%dateStr);
    std::cerr << "parseDateFromString: " << errorMsg << std::endl;
    BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
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


double msecToDays(double msecs)
{
    assert(msecs >= 0.0);

    double secs = msecs / TIME_HELPER_MSEC_PER_SEC;

    double days = secs / TIME_HELPER_SEC_PER_DAY;

    return days;
}

}

