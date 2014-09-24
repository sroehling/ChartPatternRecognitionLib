/*
 * PeriodValSegment.h
 *
 *  Created on: Jun 17, 2014
 *      Author: sroehling
 */

#ifndef PERIODVALSEGMENT_H_
#define PERIODVALSEGMENT_H_

#include <boost/shared_ptr.hpp>
#include "PeriodVal.h"
#include "LinearEquation.h"
#include "PeriodValueRef.h"

class PeriodValSegment;
typedef boost::shared_ptr<PeriodValSegment> PeriodValSegmentPtr;
typedef std::pair<PeriodValSegmentPtr,PeriodValSegmentPtr> PeriodValSegmentPair;

// PeriodValSegment is a "flyweight" pattern for accessing into and iterating over
// sequences of PeriodVal data. In general, there is only a need to keep one copy
// of the PeriodVal sequence around, but the algorithms for pattern matching will
// extensively segment and iterate over this sequence.
class PeriodValSegment {
private:
	PeriodValCltnPtr perValCltn_;
	PeriodValCltn::iterator segBegin_;
	PeriodValCltn::iterator segEnd_;
	PeriodValCltn::iterator highestHighValIter_;
	PeriodValCltn::iterator lowestLowValIter_;

	unsigned int startPos_;
	unsigned int endPos_;

	bool validPeriodValIndices() const;

	void initSegments(const PeriodValCltn::iterator &segBegin,
			const PeriodValCltn::iterator &segEnd);
	void initHighestHighValIter();
	void initLowestLowValIter();
public:
	PeriodValSegment(const PeriodValCltnPtr &perValCltn,
			const PeriodValCltn::iterator &segBegin,
			const PeriodValCltn::iterator &segEnd);

	// Construct a segment encompassing all the values
	// in perValCltn
	PeriodValSegment(const PeriodValCltnPtr &perValCltn);

	PeriodValSegmentPtr moveSegBeginToPrev() const;

	// All the values in the underlying PeriodValCltn after
	// the segment's end;
	PeriodValSegmentPtr trailingVals() const;

	// All the values in the underlying PeriodValCltn after
	// the segment's end, and including the last value if there is one.
	// This is used when multiple segments are "stitched together" for
	// purposes of pattern matching.
	PeriodValSegmentPtr trailingValsWithLastVal() const;
    PeriodValSegmentPtr trailingValsWithLastVal(unsigned int maxVals) const;

	const PeriodValCltn::iterator &segBegin() const { return segBegin_; }
	const PeriodValCltn::iterator &segEnd() const { return segEnd_; }

    // Whereby a PeriodValSegment is a "fly-weight" pointer to the same
    // underlying PeriodValCltn, perValCltn is the entire data set
    // for which this PeriodValSegment is a subset.
    const PeriodValCltnPtr perValCltn() const { return perValCltn_; }


    unsigned int segmentSpanPeriods() const;
    double segmentSpanCalendarMsec() const;
    double segmentSpanCalendarDays() const;


    // Based upon the time difference across all the periods in the underlying
    // PeriodValCltn (for all the data, not just this (sub)segment), calculate average
    // milliseconds calendar timespan per period. This is helpful for trend-line and
    // other constraints calculations based upon a slope over time rather than periods.
    double averageCalendarMsecPerPeriod() const;
    double averageCalendarDaysPerPeriod() const;
    double averageCalendarMonthsPerPeriod() const;
    double segmentSpanCalendarMonths() const;
    double averageCalendarPeriodsPerYear() const;

    // Percent change from the start to the end of the segment along the given line.
    // This is used for constraint calculations involving percent changes (e.g.,
    // for wedges and triangles).
    double perPeriodPercChangeAlongLine(const LinearEquation &line) const;
    double percChangePerYearAlongLine(const LinearEquation &line) const;

	PeriodValSegmentPair split(unsigned int splitPos) const;
	PeriodValSegmentPtr spliceRange(unsigned int startPos, unsigned int endPos) const;

    PeriodValCltn::iterator highestValIter(const PeriodValueRef &perValRef,
        const PeriodValCltn::iterator &beginRange, const PeriodValCltn::iterator &endRange) const;
    PeriodValCltn::iterator highestValIter(const PeriodValueRef &perValRef) const;
    double highestVal(const PeriodValueRef &perValRef) const;
    double highestValExceptLast(const PeriodValueRef &perValRef) const;

    PeriodValCltn::iterator lowestValIter(const PeriodValueRef &perValRef) const;
    double lowestVal(const PeriodValueRef &perValRef) const;

	unsigned int numVals() const;
	const PeriodVal &firstVal() const;
	const PeriodValCltn::iterator &firstValIter() const { return segBegin(); }
	const PeriodVal &lastVal() const;
    const PeriodValCltn::iterator lastValIter() const;

	double highestHigh() const; // Highest high within the segment
	const PeriodVal highestHighVal() const; // PeriodVal with the highest high
	const PeriodValCltn::iterator &highestHighIter() const { return highestHighValIter_; }

	double lowestLow() const; // Lowest low value within the segment
	const PeriodVal lowestLowVal() const;
	const PeriodValCltn::iterator &lowestLowIter() const { return lowestLowValIter_; }

	double depthPoints() const; // depth in number of points ($, etc.)
	double depthPercent() const; // depth from high to low, percent

	// Point value for a given percentage from high. For example, if
	// the high value is 100.0, and 'percentFromHigh' is 25, this function
	// will return 75. This is useful when constructing pattern recognition
	// constraints.
	double pointsAtPercentOfDepthBelowHigh(double percentBelowHigh) const;

	// Construct a LinearEquation from the start to finish of this segment.
	// The given PeriodValueRef determines which values on the end-points are
	// used to draw the equation.
	LinearEquationPtr segmentEquation(const PeriodValueRef &endPointValRef) const;

	static PeriodValSegmentPtr readFromFile(const std::string &fileName);


	virtual ~PeriodValSegment();
};




#endif /* PERIODVALSEGMENT_H_ */
