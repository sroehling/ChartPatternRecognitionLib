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

	typedef tokenizer< escaped_list_separator<char> > Tokenizer;
	vector< string > vec;
	string line;

	unsigned int currLineNum = 0;
	if(!getline(in,line)) // Skip the header
	{
		std::string errorMsg = boost::str(boost::format("Missing CSV header in file %s")%fileName);
		std::cerr << errorMsg << std::endl;
		BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
	}
	currLineNum++;

	// Expect data in format: Date,Open,High,Low,Close,Volume,Adj Close

	while (getline(in,line))
	{
		currLineNum++;
		Tokenizer tok(line);
		vec.assign(tok.begin(),tok.end());

		if(vec.size() != 7)
		{
			std::string errorMsg = boost::str(
					boost::format("Malformed CSV data in file %s on line %d: expecting 7 fields, got %d")
					%fileName%currLineNum%vec.size());
			std::cerr << errorMsg << std::endl;
			BOOST_THROW_EXCEPTION(std::runtime_error(errorMsg));
		}

		try
		{
			ptime perTime(date(from_simple_string(vec[0])));
			double open = lexical_cast<double>(vec[1]);
			double high = lexical_cast<double>(vec[2]);
			double low = lexical_cast<double>(vec[3]);
			double close = lexical_cast<double>(vec[4]);
			unsigned int vol = lexical_cast<unsigned int>(vec[5]);

			// TODO - Validate high >= low, etc.


			// TODO - Add adjusted closed to PeriodVal (along with adjustments to
			// normalize/adjust open,high,low,close too.
            double adjClose = lexical_cast<double>(vec[6]);


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
            vol = floor((double)vol *adjScaleFactor);

			// The data is read in reverse chronological order (most recent dates first),
			// but needs to be be in chronological order for processing
			// in memory.
			unsigned int dummyIndexPlaceholderForReassignment = 0;
			perValCltn->push_front(PeriodVal(perTime,open,high,low,close,vol,dummyIndexPlaceholderForReassignment));

		}
		catch(const std::exception &e)
		{
			std::string errorMsg = boost::str(
					boost::format("Malformed CSV data in file %s on line %d: %s")
					%fileName%currLineNum%e.what());
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
