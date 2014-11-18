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

using namespace std;
using namespace boost;
using namespace boost::posix_time;
using namespace boost::gregorian;


typedef tokenizer< escaped_list_separator<char> > CSVTokenizer;


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

struct PeriodValChronologicalSortPred {
    bool operator() (const PeriodVal &first, const PeriodVal &second)
    {
        return (first.periodTime() < second.periodTime())? true:false;
    }
};

class QuotesCSVFormatInfo
{
public:
    QuotesCSVFormatInfo()
    {
        openFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        highFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        lowFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        closeFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        volumeFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        dateFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        adjCloseFieldIndex = UNDEFINED_CSV_HEADER_INDEX;
        hasAdjustedCloseField = false;
        numHeaderFields = 0;
        firstLineIsData = false;

    }

    int openFieldIndex;
    int highFieldIndex;
    int lowFieldIndex;
    int closeFieldIndex;
    int volumeFieldIndex;
    int dateFieldIndex;
    int adjCloseFieldIndex;
    unsigned int numHeaderFields;
    bool hasAdjustedCloseField;
    bool firstLineIsData;

    bool validFormat() const
    {
        if((openFieldIndex==UNDEFINED_CSV_HEADER_INDEX) ||
            (highFieldIndex==UNDEFINED_CSV_HEADER_INDEX) ||
            (lowFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
            (closeFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
                (dateFieldIndex == UNDEFINED_CSV_HEADER_INDEX) ||
            (volumeFieldIndex == UNDEFINED_CSV_HEADER_INDEX))
        {
            return false;
        }
        if((!((numHeaderFields == 6) || (numHeaderFields==7))))
        {
            return false;
        }
        if(hasAdjustedCloseField && (adjCloseFieldIndex==UNDEFINED_CSV_HEADER_INDEX))
        {
            return false;
        }
        return true;

    }
    virtual ~QuotesCSVFormatInfo() {}
};

void parseHeaderFormat(QuotesCSVFormatInfo &quotesFormat, const std::string &line,const std::string &fileName)
{
    vector< string > headerFields;
    CSVTokenizer headerTokenizer(line);
    headerFields.assign(headerTokenizer.begin(),headerTokenizer.end());

    unsigned int numHeaderFields = headerFields.size();
    if(!((numHeaderFields == 6) || (numHeaderFields==7)))
    {
        // Need either 6 or 7 fields for date,open,high,low,close,volume, and optional adjusted volume column.
        std::string errorMsg = boost::str(boost::format("Incorrect number of fields in file %s: got %d, expecting 6 or 7")%fileName%numHeaderFields);
        std::cerr << errorMsg << std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
    }

    ptime headerTime;
    if((numHeaderFields==6) && timeHelper::findDateInString(headerFields[0],headerTime))
    {
        // There's no CSV header, so we assume the data is formatted as
        // date,open,high,low,close,volume
        quotesFormat.dateFieldIndex = 0;
        quotesFormat.openFieldIndex = 1;
        quotesFormat.highFieldIndex = 2;
        quotesFormat.lowFieldIndex = 3;
        quotesFormat.closeFieldIndex = 4;
        quotesFormat.volumeFieldIndex = 5;
        quotesFormat.numHeaderFields = 6;
        quotesFormat.firstLineIsData = true;
    }
    else
    {
        quotesFormat.numHeaderFields = 6;
        quotesFormat.firstLineIsData = false;
        // CSV File will have header fields indicating which order the data fields are expected.
        for(unsigned int headerIndex = 0; headerIndex < headerFields.size(); headerIndex++)
        {
            std::string headerFieldName = boost::to_upper_copy(headerFields[headerIndex]);
            stripNonHeaderChars(headerFieldName);

            if(headerFieldName == std::string("DATE"))
            {
                quotesFormat.dateFieldIndex = headerIndex;
            }
            else if(headerFieldName == "OPEN")
            {
                quotesFormat.openFieldIndex = headerIndex;
            }
            else if(headerFieldName == "HIGH")
            {
                quotesFormat.highFieldIndex = headerIndex;
            }
            else if(headerFieldName == "LOW")
            {
                quotesFormat.lowFieldIndex = headerIndex;
            }
            else if(headerFieldName == "CLOSE")
            {
                quotesFormat.closeFieldIndex = headerIndex;
            }
            else if(headerFieldName == "VOLUME")
            {
                quotesFormat.volumeFieldIndex = headerIndex;
            }
            else if(headerFieldName == "ADJ CLOSE")
            {
                quotesFormat.adjCloseFieldIndex = headerIndex;
                quotesFormat.hasAdjustedCloseField = true;
                quotesFormat.numHeaderFields = 7;
            }
        }

    }

    if(!quotesFormat.validFormat())
    {
        std::string errorMsg = boost::str(boost::format("Missing CSV header field(s) in file %s, got malformed header=%s")%fileName%line);
        std::cerr << errorMsg << std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
    }

}

static PeriodVal readOnePeriodVal(const QuotesCSVFormatInfo &quotesFormat, const std::string &line,
                                  unsigned int currLineNum,const std::string &fileName)
{
    assert(quotesFormat.validFormat());
    CSVTokenizer csvDataTokenizer(line);

    vector< string > csvDataFields;
    csvDataFields.assign(csvDataTokenizer.begin(),csvDataTokenizer.end());

    if(csvDataFields.size() != quotesFormat.numHeaderFields)
    {
        std::string errorMsg = boost::str(
                boost::format("Malformed CSV data in file %s on line %d: expecting %d fields, got %d")
                %fileName%currLineNum%quotesFormat.numHeaderFields%csvDataFields.size());
        std::cerr << errorMsg << std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
    }

    std::string dateField = csvDataFields[quotesFormat.dateFieldIndex];
    ptime perTime(timeHelper::parseDateFromString(dateField));

    double open = lexical_cast<double>(csvDataFields[quotesFormat.openFieldIndex]);
    double high = lexical_cast<double>(csvDataFields[quotesFormat.highFieldIndex]);
    double low = lexical_cast<double>(csvDataFields[quotesFormat.lowFieldIndex]);
    double close = lexical_cast<double>(csvDataFields[quotesFormat.closeFieldIndex]);
    unsigned int vol = lexical_cast<unsigned int>(csvDataFields[quotesFormat.volumeFieldIndex]);

    // TODO - Validate high >= low, etc.

    if(quotesFormat.hasAdjustedCloseField)
    {
        double adjClose = lexical_cast<double>(csvDataFields[quotesFormat.adjCloseFieldIndex]);

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
    unsigned int dummyIndexPlaceholderForReassignment = 0;
    return PeriodVal(perTime,open,high,low,close,vol,dummyIndexPlaceholderForReassignment);

}


PeriodValCltnPtr PeriodVal::readFromFile(const std::string &fileName)
{

	ifstream in(fileName.c_str());

	PeriodValCltnPtr perValCltn(new PeriodValCltn());

	if (!in.is_open())
	{
		BOOST_THROW_EXCEPTION(
			std::runtime_error(
				boost::str(boost::format("Unable to open file %s")%fileName)));
	}

	string line;
	unsigned int currLineNum = 0;

    // ---------------------------------------------------------------------------------------------
    // Parse the CSV header
    // ---------------------------------------------------------------------------------------------

    if(!getline(in,line))
	{
		std::string errorMsg = boost::str(boost::format("Missing CSV header in file %s")%fileName);
		std::cerr << errorMsg << std::endl;
		BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
	}
    currLineNum++;
    QuotesCSVFormatInfo quotesFormat;
    parseHeaderFormat(quotesFormat,line,fileName);

    // ---------------------------------------------------------------------------------------------
    // Parse the open,high,low,close,volume (OHLC) data
    // ---------------------------------------------------------------------------------------------

    if(quotesFormat.firstLineIsData)
    {
        PeriodVal currLinePerVal = readOnePeriodVal(quotesFormat,line,currLineNum,fileName);
        perValCltn->push_front(currLinePerVal);
    }

	while (getline(in,line))
	{
        currLineNum++;
        PeriodVal currLinePerVal = readOnePeriodVal(quotesFormat,line,currLineNum,fileName);
        perValCltn->push_front(currLinePerVal);
	}

    // Sort in chronological order (earliest date first, most recent date last)
    perValCltn->sort(PeriodValChronologicalSortPred());
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
