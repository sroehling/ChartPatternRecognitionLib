/*
 * PeriodVal.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PeriodVal.h"

#include <stdlib.h>
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "TimeHelper.h"

#define UNDEFINED_CSV_HEADER_INDEX -1


PeriodVal::PeriodVal(boost::posix_time::ptime &periodTime,
		double open, double high, double low,
			double close, unsigned int volume,unsigned int perValIndex)
: periodTime_(periodTime),
  	  open_(open), high_(high), low_(low), close_(close), volume_(volume),
	perValIndex_(perValIndex)
{
}

double PeriodVal::typicalPrice() const
{
	return (high() + low() + close()) / 3.0;
}

PeriodVal::~PeriodVal() {
}

static bool notCSVHeaderChar(char c)
{
    if(isalpha(c) || c==' ')
    {
        return false;
    }
    else
    {
        return true;
    }
}

static void stripNonHeaderChars(std::string &str)
{
    str.erase(std::remove_if(str.begin(), str.end(), notCSVHeaderChar), str.end());
}

PeriodValCltnPtr PeriodVal::readFromFile(const std::string &fileName)
{
	using namespace std;
	using namespace boost;
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ifstream in(fileName.c_str());

	PeriodValCltnPtr perValCltn(new PeriodValCltn());

	if (!in.is_open())
	{
		BOOST_THROW_EXCEPTION(
			std::runtime_error(
				boost::str(boost::format("Unable to open file %s")%fileName)));
	}

    typedef tokenizer< escaped_list_separator<char> > CSVTokenizer;

	string line;
	unsigned int currLineNum = 0;


    // ---------------------------------------------------------------------------------------------
    // Parse the CSV header
    // ---------------------------------------------------------------------------------------------

    // Each of the required fields are
    int openFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int highFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int lowFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int closeFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int volumeFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int dateFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    int adjCloseFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
    bool hasAdjustedCloseField = false;
    if(!getline(in,line))
	{
		std::string errorMsg = boost::str(boost::format("Missing CSV header in file %s")%fileName);
		std::cerr << errorMsg << std::endl;
		BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
	}
    vector< string > headerFields;
    CSVTokenizer headerTokenizer(line);
    headerFields.assign(headerTokenizer.begin(),headerTokenizer.end());

    unsigned int numHeaderFields = headerFields.size();

    for(unsigned int headerIndex = 0; headerIndex < headerFields.size(); headerIndex++)
    {
        std::string headerFieldName = boost::to_upper_copy(headerFields[headerIndex]);
        stripNonHeaderChars(headerFieldName);

        if(headerFieldName == std::string("DATE"))
        {
            dateFieldIndex = headerIndex;
        }
        else if(headerFieldName == "OPEN")
        {
            openFieldIndex = headerIndex;
        }
        else if(headerFieldName == "HIGH")
        {
            highFieldIndex = headerIndex;
        }
        else if(headerFieldName == "LOW")
        {
            lowFieldIndex = headerIndex;
        }
        else if(headerFieldName == "CLOSE")
        {
            closeFieldIndex = headerIndex;
        }
        else if(headerFieldName == "VOLUME")
        {
            volumeFieldIndex = headerIndex;
        }
        else if(headerFieldName == "ADJ CLOSE")
        {
            adjCloseFieldIndex = headerIndex;
            hasAdjustedCloseField = true;
        }
    }
    if((openFieldIndex==UNDEFINED_CSV_HEADER_INDEX) ||
        (highFieldIndex==UNDEFINED_CSV_HEADER_INDEX) ||
        (lowFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
        (closeFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
            (dateFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
        (volumeFieldIndex == UNDEFINED_CSV_HEADER_INDEX))
    {
        std::string errorMsg = boost::str(boost::format("Missing CSV header field(s) in file %s, got malformed header=%s")%fileName%line);
        std::cerr << errorMsg << std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
    }

    // ---------------------------------------------------------------------------------------------
    // End header parsing
    // ---------------------------------------------------------------------------------------------

	currLineNum++;

	// Expect data in format: Date,Open,High,Low,Close,Volume,Adj Close

	while (getline(in,line))
	{
		currLineNum++;
        CSVTokenizer csvDataTokenizer(line);

        vector< string > csvDataFields;
        csvDataFields.assign(csvDataTokenizer.begin(),csvDataTokenizer.end());

        if(csvDataFields.size() != numHeaderFields)
		{
			std::string errorMsg = boost::str(
                    boost::format("Malformed CSV data in file %s on line %d: expecting %d fields, got %d")
                    %fileName%currLineNum%numHeaderFields%csvDataFields.size());
			std::cerr << errorMsg << std::endl;
			BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
		}

		try
		{
            std::string dateField = csvDataFields[dateFieldIndex];
            ptime perTime(timeHelper::parseDateFromString(dateField));

            double open = lexical_cast<double>(csvDataFields[openFieldIndex]);
            double high = lexical_cast<double>(csvDataFields[highFieldIndex]);
            double low = lexical_cast<double>(csvDataFields[lowFieldIndex]);
            double close = lexical_cast<double>(csvDataFields[closeFieldIndex]);
            unsigned int vol = lexical_cast<unsigned int>(csvDataFields[volumeFieldIndex]);

			// TODO - Validate high >= low, etc.

            if(hasAdjustedCloseField)
            {
                double adjClose = lexical_cast<double>(csvDataFields[adjCloseFieldIndex]);


                // "Normalize" the data based upon the adjusted close value. Technical
                // analysis and back-testing should always used adjusted values.
                if(close <= 0.0)
                {
                    std::string errorMsg = boost::str(
                            boost::format("Malformed CSV data in file %s on line %d: expecting close >= 0.0, got %f")
                            %fileName%currLineNum%close);
                    std::cerr << errorMsg << std::endl;
                    BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
                }

                double adjScaleFactor = adjClose/close;
                low = low*adjScaleFactor;
                high = high*adjScaleFactor;
                open = open*adjScaleFactor;
                close = close*adjScaleFactor;

                // Note that we *do not* adjust volume like the other fields. For Yahoo Finance EOD data,
                // in particular, the volume is already adjusted. This was verified with by looking at the
                // unadjusted and adjusted charts on stockcharts.com. On 2014-06-06, the unadjusted volume was
                // 12M, or 87M adjusted. By comparison, the EOD from Google adjusts all the columns by default.
            }

			// The data is read in reverse chronological order (most recent dates first),
			// but needs to be be in chronological order for processing
			// in memory.
			unsigned int dummyIndexPlaceholderForReassignment = 0;
			perValCltn->push_front(PeriodVal(perTime,open,high,low,close,vol,dummyIndexPlaceholderForReassignment));

		}
		catch(const std::exception &e)
		{
			std::string errorMsg = boost::str(
                    boost::format("Malformed CSV data in file %s on line %d: %s, csv data =%s")
                    %fileName%currLineNum%e.what()%line);
			std::cerr << errorMsg << std::endl;
			BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
		}

	}
	PeriodVal::reAssignIndices(*perValCltn);

	return perValCltn;
}

void PeriodVal::reAssignIndices(PeriodValCltn &perValCltn)
{
	unsigned int currIndex = 0;
	for(PeriodValCltn::iterator perValIter = perValCltn.begin(); perValIter != perValCltn.end(); perValIter++)
	{
		(*perValIter).setIndex(currIndex);
		currIndex++;
	}
}

std::ostream& operator<<(std::ostream& os, const PeriodVal& perVal)
{
	os << "PeriodVal("
			<< "time="<<perVal.periodTime()
			<< ",open="<<perVal.open_
			<< ", high="<<perVal.high_
			<< ", low="<<perVal.low_
			<< ", close="<<perVal.close_
			<< ", typical="<< perVal.typicalPrice()
			<< ", vol="<<perVal.volume_
			<< ")";
	return os;

}
