/*
 * PeriodData.cpp
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#include "PeriodData.h"

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

PeriodData::PeriodData(boost::posix_time::ptime &periodTime,
		double open, double high, double low,
			double close, unsigned int volume)
: periodTime_(periodTime),
  	  open_(open), high_(high), low_(low), close_(close), volume_(volume)
{
}

PeriodData::~PeriodData() {
}


PeriodDataCltnPtr PeriodData::readFromFile(const std::string &fileName)
{
	using namespace std;
	using namespace boost;
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	ifstream in(fileName.c_str());

	PeriodDataCltnPtr perDataCltn(new PeriodDataCltn());

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
			double adjClose = lexical_cast<double>(vec[6]);

			// The data is read in reverse chronological order (most recent dates first),
			// but needs to be be in chronological order for processing
			// in memory.
			perDataCltn->push_front(PeriodData(perTime,open,high,low,close,vol));

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
	return perDataCltn;
}


std::ostream& operator<<(std::ostream& os, const PeriodData& perData)
{
	os << "PeriodData("
			<< "open="<<perData.open_
			<< ", high="<<perData.high_
			<< ", low="<<perData.low_
			<< ", close="<<perData.close_
			<< ", vol="<<perData.volume_
			<< ")";
	return os;

}
