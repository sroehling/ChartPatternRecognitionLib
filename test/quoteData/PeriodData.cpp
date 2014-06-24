/*
 * PeriodData.cpp
 *
 *  Created on: Jun 23, 2014
 *      Author: sroehling
 */


#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;


#include "PeriodData.h"

BOOST_AUTO_TEST_CASE( ReadPeriodDataTest )
{
	// Read a valid quotes file and check some of the values
	PeriodDataCltnPtr periodData = PeriodData::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	BOOST_CHECK(periodData->size()==27);

	PeriodData firstVal = periodData->front();
	BOOST_CHECK_EQUAL(firstVal.volume(),1323000);
	BOOST_CHECK_CLOSE(firstVal.close(),71.84,0.001);

	PeriodData lastVal = periodData->back();
	BOOST_CHECK_EQUAL(lastVal.volume(),3770100);
	BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,2,18)));

	// Verify error conditions are handled by the quote reading code.
	BOOST_CHECK_THROW(PeriodData::readFromFile("invalidFileName.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodData::readFromFile("./quoteData/MissingHeader.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodData::readFromFile("./quoteData/MissingField.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodData::readFromFile("./quoteData/CorruptDate.csv"),std::runtime_error);
	BOOST_CHECK_THROW(PeriodData::readFromFile("./quoteData/CorruptVal.csv"),std::runtime_error);
}

