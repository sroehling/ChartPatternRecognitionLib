/*
 * TrendLineScanner.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#include "TrendLineScanner.h"

#include "SegmentConstraint.h"
#include "SlopeWithinRangeConstraint.h"
#include "SegmentValsCloseToLinearEq.h"
#include "ANDSegmentConstraint.h"
#include "AnySegmentListValidConstraint.h"
#include "AnyPatternMatchValidator.h"
#include "PeriodValSegment.h"
#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"

const DoubleRange TrendLineScanner::UPTREND_SLOPE_RANGE(0.5,15.0);
const DoubleRange TrendLineScanner::DOWNTREND_SLOPE_RANGE(-15.0,-0.5);
const DoubleRange TrendLineScanner::FLAT_SLOPE_RANGE(-0.5,0.5);

void TrendLineScanner::initTrendScanner(const DoubleRange &slopeRange,
		const PatternMatchValidatorPtr &matchConstraint, double maxPercDistanceToLineEquation)
{
	SegmentConstraintPtr valsCloseToEquation(new SegmentValsCloseToLinearEq(maxPercDistanceToLineEquation));
	SegmentConstraintPtr trendSlope(new SlopeWithinRangeConstraint(slopeRange));

	SegmentConstraintList trendConstraints;
	trendConstraints.push_back(valsCloseToEquation);
	trendConstraints.push_back(trendSlope);
	SegmentConstraintPtr trendSegConstraints(new ANDSegmentConstraint(trendConstraints));

	SegmentListConstraintPtr trendSegListConstraint(new AnySegmentListValidConstraint());

	PatternMatchValidatorPtr trendPatternMatchValidator(matchConstraint);

	trendScanner_ = PatternScannerPtr(new PatternScannerEngine(trendSegConstraints,trendSegListConstraint,trendPatternMatchValidator));

}

TrendLineScanner::TrendLineScanner(const DoubleRange &slopeRange, double maxPercDistToLineEquation)
{
	initTrendScanner(slopeRange,PatternMatchValidatorPtr(new AnyPatternMatchValidator()),maxPercDistToLineEquation);

}


TrendLineScanner::TrendLineScanner(const DoubleRange &slopeRange, const PatternMatchValidatorPtr &matchConstraint) {

	initTrendScanner(slopeRange,matchConstraint,7.0);

}

TrendLineScanner::TrendLineScanner(const DoubleRange &slopeRange) {

	initTrendScanner(slopeRange,PatternMatchValidatorPtr(new AnyPatternMatchValidator()),7.0);
}


PatternMatchListPtr TrendLineScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	// TODO - Depending on the slope range, need to validate no value goes below or above the first value.

	return trendScanner_->scanPatternMatches(chartVals);
}


TrendLineScanner::~TrendLineScanner() {
}

