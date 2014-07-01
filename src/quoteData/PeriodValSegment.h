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

	unsigned int startPos_;
	unsigned int endPos_;

	void initSegments(const PeriodValCltn::iterator &segBegin,
			const PeriodValCltn::iterator &segEnd);
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

	const PeriodValCltn::iterator &segBegin() const { return segBegin_; }
	const PeriodValCltn::iterator &segEnd() const { return segEnd_; }
	const PeriodValCltnPtr perValCltn() const { return perValCltn_; }

	PeriodValSegmentPair split(unsigned int splitPos) const;

	unsigned int numVals() const;
	const PeriodVal &firstVal() const;
	const PeriodVal &lastVal() const;

	double highestHigh() const; // Highest high within the segment
	double lowestLow() const; // Lowest low value within the segment
	double depthPoints() const; // depth in number of points ($, etc.)
	double depthPercent() const; // depth from high to low, percent

	// Point value for a given percentage from high. For example, if
	// the high value is 100.0, and 'percentFromHigh' is 25, this function
	// will return 75. This is useful when constructing pattern recognition
	// constraints.
	double pointsAtPercentOfDepthBelowHigh(double percentBelowHigh) const;

	virtual ~PeriodValSegment();
};




#endif /* PERIODVALSEGMENT_H_ */
