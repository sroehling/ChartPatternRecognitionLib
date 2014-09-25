/*
 * PatternMatch.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCH_H_
#define PATTERNMATCH_H_

#include <boost/shared_ptr.hpp>
#include "ChartSegment.h"
#include "PatternMatchBreakoutInfo.h"

class PatternMatchVisitor;

class PatternMatch;
typedef boost::shared_ptr<PatternMatch> PatternMatchPtr;
typedef std::list<PatternMatchPtr> PatternMatchList;
typedef std::vector<PatternMatchPtr> PatternMatchVector;
typedef boost::shared_ptr<PatternMatchList> PatternMatchListPtr;


class PatternMatch {
private:
	ChartSegmentList segments_;
	bool segmentsConnected(const ChartSegmentList &segments) const;

	// Construct a combined segment from the start to the end of the
	// pattern match. This is useful for performing internal calculations
	// on pattern depth, lowest lows, etc.
	void initMatchSegment();
	PeriodValSegmentPtr matchSegment_;

protected:
    // Method for sub-classes to initialize the breakoutInfo when the last
    // value cloases above the first high.
    void initBreakoutAboveFirstHighBreakoutInfo(const PatternMatchPtr &match);

public:
	PatternMatch(const ChartSegmentList &segments);
	PatternMatch(const ChartSegmentPtr &singleSegment);

	const PeriodVal &firstValue() const;
	const PeriodVal &lastValue() const;
    const PeriodVal &secondToLastValue() const;

	const boost::posix_time::ptime &startTime() const;
	const boost::posix_time::ptime &endTime() const;

    const PeriodValCltn::iterator &beginMatchIter() const;
    const PeriodValCltn::iterator &endMatchIter() const;



    const PeriodValSegmentPtr  &matchSegment() const { return matchSegment_; }

	unsigned int numSegments() const;
	unsigned int numPeriods() const;
	const ChartSegmentList &segments() const;

	double highestHigh() const; // Highest high across all segments
	PeriodVal highestHighVal() const; // Full PeriodVal with the highest high
	const boost::posix_time::ptime highestHighTime() const; // Time when the highest high took place - good for sorting/unique
	const PeriodValCltn::iterator &highestHighIter() const;

	double lowestLow() const; // Lowest low across all segments
	PeriodVal lowestLowVal() const; // Full PeriodVal with the lowest low
	const boost::posix_time::ptime lowestLowTime() const; // Time when the lowest low took place - good for sorting/unique
	const PeriodValCltn::iterator &lowestLowIter() const;

	double depthPoints() const; // depth from highest high to lowest low
	double depthPercent() const; // depth from highest high to lowest low

	double pointsAtPercentOfDepthBelowHigh(double percentFromHigh) const;

	// Return a segment containing all the PeriodVal's after this
	// pattern match, and including the last value. This is needed
	// for matching sub-patterns, then putting/stitching together
	// the sub-patterns into a combined pattern.
	PeriodValSegmentPtr trailingValsWithLastVal() const;
    PeriodValSegmentPtr trailingValsWithLastVal(unsigned int maxVals) const;


	PatternMatchPtr appendMatch(const PatternMatch &matchToAppend) const;
	PatternMatchListPtr appendMatchList(const PatternMatchList &appendList) const;

	friend std::ostream& operator<<(std::ostream& os, const PatternMatch& patternMatch);

    virtual void acceptVisitor(PatternMatchVisitor &) {}

    virtual std::string matchType() const { return "Match Type TBD"; }

    // Optional, associated breakout information. Can can be NULL if
    // no break-out is associated.
    PatternMatchBreakoutInfoPtr breakoutInfo;
    PatternMatchBreakoutInfoPtr breakdownInfo;

    // Match with a breakout or breakdown?
    bool isConfirmedMatch() const;

	virtual ~PatternMatch();
};


#endif /* PATTERNMATCH_H_ */
