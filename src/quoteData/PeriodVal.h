/*
 * PeriodVal.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PERIODVAL_H_
#define PERIODVAL_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <ostream>
#include "XYCoord.h"

class PeriodVal;
typedef std::list<PeriodVal> PeriodValCltn;
typedef boost::shared_ptr<PeriodValCltn> PeriodValCltnPtr;


class PeriodVal {
private:
	boost::posix_time::ptime periodTime_;
	double open_;
	double high_;
	double low_;
	double close_;
	unsigned int volume_;

	// Each PeriodVal needs to be assigned a unique index value w.r.t the
	// other PeriodVal's in the same collection. A "flyweight" iterator is
	// used to access all the PeriodVal's, and this index is used for
	// calculations with LinearEquations, since a date cannot be used
	// as an X value in a LinearEquation.
	unsigned int perValIndex_;

	// setIndex is for internal use by code for reading PeriodVal's
	void setIndex(unsigned int index) { perValIndex_ = index; }


public:
	PeriodVal(boost::posix_time::ptime &periodTime,
			double open, double high, double low, double close, unsigned int volume,
			unsigned int perValIndex);

	const boost::posix_time::ptime &periodTime() const { return periodTime_; }
	double open() const { return open_; }
	double high() const { return high_; }
	double low() const { return low_; }
	double close() const { return close_; }
	unsigned int volume() const { return volume_; }

	unsigned int perValIndex() const { return perValIndex_; }
	double pseudoXVal() const { return (double)perValIndex_; }

	// Coordinate based upon psuedoXVal and the closing price. This
	// is used to test the closing price for cross-overs of a LinearEquation's line.
	// (e.g. for wedge pattern matching).
	XYCoord closeCoord() const { return XYCoord(pseudoXVal(),close()); }

	static void reAssignIndices(PeriodValCltn &perValCltn);

	static PeriodValCltnPtr readFromFile(const std::string &fileName);

	double typicalPrice() const;

	friend std::ostream& operator<<(std::ostream& os, const PeriodVal& perVal);

	virtual ~PeriodVal();
};



#endif /* PERIODVAL_H_ */
