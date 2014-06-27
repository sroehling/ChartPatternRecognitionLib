/*
 * TestHelper.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "TestHelper.h"

using namespace boost::posix_time;
using namespace boost::gregorian;


PeriodVal testPeriodVal(unsigned int year, unsigned int month, unsigned int day, double val, unsigned int vol)
{
	ptime perTime(date(year,month,day));
	PeriodVal perVal(perTime,val,val,val,val,vol);
	return perVal;
}

