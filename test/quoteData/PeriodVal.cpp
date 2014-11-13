/*
 * PeriodData.cpp
 *
 *  Created on: Jun 23, 2014
 *      Author: sroehling
 */


#include <boost/test/unit_test.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "PeriodValueRef.h"

using namespace boost::posix_time;
using namespace boost::gregorian;


#include "PeriodVal.h"

BOOST_AUTO_TEST_CASE( PeriodValTest_WithAdjustedValues )
{
	// Read a valid quotes file and check some of the values
	PeriodValCltnPtr periodData = PeriodVal::readFromFile("./patternScan/QCOR_DoubleBottom_Weekly.csv");
	BOOST_CHECK(periodData->size()==26);

	PeriodVal firstVal = periodData->front();
    // Note - both the volume and close values are adjusted, based
    // upon the difference between the close and adjusted close.
    BOOST_CHECK_EQUAL(firstVal.volume(),1449323);
    BOOST_CHECK_CLOSE(firstVal.close(),65.76,0.001);

	ClosePeriodValueRef closeRef;
    BOOST_CHECK_CLOSE(closeRef.referencedVal(firstVal),65.76,0.001);

	PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.volume(),3755681);
	BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,2,18)));

}

BOOST_AUTO_TEST_CASE( PeriodValTest_InvalidCSV )
{
    // Verify error conditions are handled by the quote reading code.
    BOOST_CHECK_THROW(PeriodVal::readFromFile("invalidFileName.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MissingHeader.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MissingField.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/CorruptDate.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/CorruptVal.csv"),std::runtime_error);
}

BOOST_AUTO_TEST_CASE( PeriodValTest_ValidCSV )
{
    PeriodValCltnPtr periodData = PeriodVal::readFromFile("./quoteData/yahooFormatAAPL.csv");
    BOOST_TEST_MESSAGE( "Yahoo formatted CSV Data: number of rows: " << periodData->size() );
    BOOST_CHECK(periodData->size()==203);

    periodData = PeriodVal::readFromFile("./quoteData/GoogleFormatAAPL.csv");
    BOOST_TEST_MESSAGE( "Google formatted CSV Data: number of rows - AAPL: " << periodData->size() );
    BOOST_CHECK(periodData->size()==250);
    PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.volume(),27442252);
    BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,11,11)));

    periodData = PeriodVal::readFromFile("./quoteData/GoogleFormatAAPL-Short.csv");
    BOOST_CHECK(periodData->size()==2);
    lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,11,10)));
    PeriodVal firstVal = periodData->front();
    BOOST_CHECK_EQUAL(firstVal.periodTime(),ptime(date(2014,11,07)));

    periodData = PeriodVal::readFromFile("./quoteData/GoogleFormatCELG.csv");
    BOOST_TEST_MESSAGE( "Google formatted CSV Data - CELG: number of rows: " << periodData->size() );
    BOOST_CHECK(periodData->size()==252);

}

BOOST_AUTO_TEST_CASE( PeriodValTest_HeaderErrors )
{
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MalformedHeader.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MalformedHeaderNoCloseField.csv"),std::runtime_error);
}



