/*
 * DateHelper.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include "TimeHelper.h"
#include <sstream>

TimeHelper::TimeHelper() {
}


std::string TimeHelper::formatDate(const boost::posix_time::ptime &dateToFormat)
{
  using namespace boost::posix_time;


  time_facet *facet = new time_facet("%Y-%m-%d");
  std::ostringstream oss;
  oss.imbue(std::locale(oss.getloc(), facet));

  oss << dateToFormat;

  return oss.str();
}

TimeHelper::~TimeHelper() {
}

