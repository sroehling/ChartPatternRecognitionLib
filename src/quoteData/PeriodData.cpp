/*
 * PeriodData.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PeriodData.h"

PeriodData::PeriodData(boost::posix_time::ptime &periodTime,
		double open, double high, double low,
			double close, unsigned int volume)
: periodTime_(periodTime),
  	  open_(open), high_(high), low_(low), close_(close), volume_(volume)
{
	// TODO Auto-generated constructor stub

}

PeriodData::~PeriodData() {
	// TODO Auto-generated destructor stub
}

