/*
 * ChartSegment.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef CHARTSEGMENT_H_
#define CHARTSEGMENT_H_

#include <boost/shared_ptr.hpp>

#include "PeriodValSegment.h"
#include "LinearEquation.h"

// TODO Change name to TrendLine or similar - This class is used as more of a trend line
// than "Chart Segment".
class ChartSegment {
private:
	PeriodValSegmentPtr segmentVals_;
	LinearEquationPtr segmentEq_;
	PeriodValueRefPtr perValRef_;

	double absRelPercentVal(double comparisonVal, double baseVal) const;
public:
	ChartSegment(const PeriodValSegmentPtr &segmentVals);

	ChartSegment(const PeriodValCltnPtr &perValCltn,
			const PeriodValCltn::iterator &startPt,
			const PeriodValCltn::iterator &endPt,
			const PeriodValueRefPtr &endPtValueRef);

	const PeriodVal &lastPeriodVal() const;
    const PeriodValCltn::iterator lastValIter() const;

	const PeriodVal &firstPeriodVal() const;
	const PeriodValCltn::iterator &firstValIter() const;

	double maxRelPercentVsLinearEq() const;
	unsigned int numPeriods() const;
	const PeriodValSegmentPtr &perValSegment() const { return segmentVals_; }
	const LinearEquationPtr &segmentEq() const { return segmentEq_; }

	double slope() const;

    // Relative to the first value, how much the price is changing per year?
    // This CAGR serves as a "normalized" slope calculation which can be useful for trendline
    // slope validation. These values are per calendar year, not trading days/weeks.
    // Using %change in price is considered a more normalized slope calculation,
    // since the regular slope() function returns a slope dependent upon whatever the
    // price range is for the given data set.
    double percentChangePerYear() const;

	friend std::ostream& operator<<(std::ostream& os, const ChartSegment& chartSeg);


	virtual ~ChartSegment();
};

typedef boost::shared_ptr<ChartSegment> ChartSegmentPtr;
typedef std::vector<ChartSegmentPtr> ChartSegmentList;

#endif /* CHARTSEGMENT_H_ */
