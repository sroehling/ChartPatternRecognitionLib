/*
 * WedgeScanner.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>

#include <WedgeScanner.h>
#include "ChartSegment.h"

WedgeScanner::WedgeScanner() {
	minPercDistanceToUpperLowerTrendlineIntercept_ = 0.6;
	minPercValsBetweenTrendlines_ = 0.85;
}

double WedgeScanner::numPeriodsToIntercept(const ChartSegmentPtr &upperTrendLine,const ChartSegmentPtr &lowerTrendLine) const
{
	double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();

	XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));
	double numPeriodsToIntercept = trendlineIntercept.x() - firstPivotHighXVal;
	assert(numPeriodsToIntercept > 0.0);

	return numPeriodsToIntercept;
}



bool WedgeScanner::validTrendLines(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	if(lowerTrendLine->segmentEq()->slope() != upperTrendLine->segmentEq()->slope())
	{
		XYCoord trendlineIntercept = lowerTrendLine->segmentEq()->intercept(*(upperTrendLine->segmentEq()));

		// Only continue if the intercept occurs after the first pivot high's "pseudo X" value
		// (i.e., the unique numerical value assigned for each PeriodVal's date). If the intercept
		// is before the first pivot high, then the lines are angled away from each other, and
		// we're dealing with a "megaphone" type pattern (which may also be a valid pattern match
		// at some point, but not here).
		double firstPivotHighXVal = upperTrendLine->firstPeriodVal().pseudoXVal();
		if(trendlineIntercept.x() > firstPivotHighXVal)
		{
			BOOST_LOG_TRIVIAL(debug) << "WedgeScanner: "
					<< " upper trend line: " << upperTrendLine
					<< " lower trend line: " << lowerTrendLine
					<< " intercept " << trendlineIntercept << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


unsigned int WedgeScanner::minPatternPeriods(const ChartSegmentPtr &upperTrendLine,
		const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));
	double numPeriodsToIntercept = this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine);

	double minPeriods = floor(minPercDistanceToUpperLowerTrendlineIntercept_ * numPeriodsToIntercept);

	return minPeriods;
}

unsigned int WedgeScanner::maxPatternPeriods(const ChartSegmentPtr &upperTrendLine, const ChartSegmentPtr &lowerTrendLine) const
{
	assert(validTrendLines(upperTrendLine,lowerTrendLine));

	unsigned int maxPeriods = floor(this->numPeriodsToIntercept(upperTrendLine,lowerTrendLine));
	BOOST_LOG_TRIVIAL(debug) << "WedgeScannerEngine: num periods to intercept: "
			<< maxPeriods << std::endl;

	return maxPeriods;
}



WedgeScanner::~WedgeScanner() {
}

