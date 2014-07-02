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

void TrendLineScanner::initTrendScanner(double minSlope, double maxSlope,
		const PatternMatchValidatorPtr &matchConstraint, double maxPercDistanceToLineEquation)
{
	SegmentConstraintPtr valsCloseToEquation(new SegmentValsCloseToLinearEq(maxPercDistanceToLineEquation));
	SegmentConstraintPtr trendSlope(new SlopeWithinRangeConstraint(minSlope,maxSlope));

	SegmentConstraintList trendConstraints;
	trendConstraints.push_back(valsCloseToEquation);
	trendConstraints.push_back(trendSlope);
	SegmentConstraintPtr trendSegConstraints(new ANDSegmentConstraint(trendConstraints));

	SegmentListConstraintPtr trendSegListConstraint(new AnySegmentListValidConstraint());

	PatternMatchValidatorPtr trendPatternMatchValidator(matchConstraint);

	trendScanner_ = PatternScannerPtr(new PatternScannerEngine(trendSegConstraints,trendSegListConstraint,trendPatternMatchValidator));

}

TrendLineScanner::TrendLineScanner(double minSlope, double maxSlope, double maxPercDistToLineEquation)
{
	initTrendScanner(minSlope,maxSlope,PatternMatchValidatorPtr(new AnyPatternMatchValidator()),maxPercDistToLineEquation);

}


TrendLineScanner::TrendLineScanner(double minSlope, double maxSlope, const PatternMatchValidatorPtr &matchConstraint) {

	initTrendScanner(minSlope,maxSlope,matchConstraint,7.0);

}

TrendLineScanner::TrendLineScanner(double minSlope, double maxSlope) {

	initTrendScanner(minSlope,maxSlope,PatternMatchValidatorPtr(new AnyPatternMatchValidator()),7.0);
}


PatternMatchListPtr TrendLineScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	return trendScanner_->scanPatternMatches(chartVals);
}


TrendLineScanner::~TrendLineScanner() {
}

