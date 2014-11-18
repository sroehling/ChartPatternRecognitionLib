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
    // The close values are adjusted, based
    // upon the difference between the close and adjusted close.
    // Since this test data came from Yahoo Finance, however, the
    // volume is also adjusted.
    //
    // The values from the CSV file are as follows:
    // Date,Open,High,Low,Close,Volume,Adj Close
    // 2013-08-26,72.41,74.76,66.01,66.68,1469600,65.76
    //
    // The "scale factor to adjust the open, high, and low is
    // as follows: (from PerVal.cpp source):
    //      double adjScaleFactor = adjClose/close;
    // So, in this case, we expect the scale factor to be: 65.76/66.68 = 0.9862
    double scaleFactor = 65.76/66.68;
    BOOST_CHECK_EQUAL(firstVal.volume(),1469600);
    BOOST_CHECK_CLOSE(firstVal.close(),65.76,0.01);
    BOOST_CHECK_CLOSE(firstVal.open(),scaleFactor*72.41,0.01);
    BOOST_CHECK_CLOSE(firstVal.high(),scaleFactor*74.76,0.01);
    BOOST_CHECK_CLOSE(firstVal.low(),scaleFactor*66.01,0.01);


	ClosePeriodValueRef closeRef;
    BOOST_CHECK_CLOSE(closeRef.referencedVal(firstVal),65.76,0.001);

    // The values from the CSV file are as follows:
    // Date,Open,High,Low,Close,Volume,Adj Close
    // 2014-02-18,70.37,76.46,69.73,75.83,3770100,75.54
    PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.volume(),3770100);
    BOOST_CHECK_CLOSE(lastVal.close(),75.54,0.001);
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

BOOST_AUTO_TEST_CASE( PeriodValTest_ValidCSV_yLoader )
{
    PeriodValCltnPtr periodData = PeriodVal::readFromFile("./quoteData/yLoaderFormatAAPL.csv");
    BOOST_TEST_MESSAGE( "yLoader formatted CSV Data - AAPL: number of rows: " << periodData->size() );
    BOOST_CHECK(periodData->size()==285);
    PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,11,14)));
    PeriodVal firstVal = periodData->front();
    BOOST_CHECK_EQUAL(firstVal.periodTime(),ptime(date(2013,10,01)));

}

BOOST_AUTO_TEST_CASE( PeriodValTest_ValidCSV_eodData )
{
    PeriodValCltnPtr periodData = PeriodVal::readFromFile("./quoteData/eodDataFormat_AAPL_20140101_20141117.csv");
    BOOST_TEST_MESSAGE( "eod data formatted CSV Data - AAPL: number of rows: " << periodData->size() );
    BOOST_CHECK(periodData->size()==229);
    PeriodVal lastVal = periodData->back();
    BOOST_CHECK_EQUAL(lastVal.periodTime(),ptime(date(2014,11,17)));
    BOOST_CHECK_EQUAL(lastVal.volume(),46746700);
    PeriodVal firstVal = periodData->front();
    BOOST_CHECK_EQUAL(firstVal.periodTime(),ptime(date(2014,01,01)));

}


BOOST_AUTO_TEST_CASE( PeriodValTest_MalformedHeaders )
{
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MalformedHeader.csv"),std::runtime_error);
    BOOST_CHECK_THROW(PeriodVal::readFromFile("./quoteData/MalformedHeaderNoCloseField.csv"),std::runtime_error);
}



