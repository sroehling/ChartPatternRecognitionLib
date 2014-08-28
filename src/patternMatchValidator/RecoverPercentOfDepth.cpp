/*
 * RecoverPercentOfDepth.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#include <RecoverPercentOfDepth.h>
#include "PeriodValueRef.h"
#include <boost/log/trivial.hpp>
#include "DoubleRange.h"
#include "LastValueAbovePointValue.h"
#include "PeriodValueRef.h"

RecoverPercentOfDepth::RecoverPercentOfDepth(double depthPercRecover)
: depthPercRecover_(depthPercRecover)
{
	assert(DoubleRange(0.0,100.0).valueWithinRange(depthPercRecover));
}


PatternMatchValidatorPtr RecoverPercentOfDepth::createValidator(
		const PatternMatchVector &previousMatches) const
{
    // This validator always compares against the first previous match.
    assert(previousMatches.size() > 0);
	PatternMatchPtr otherMatch = previousMatches[0];

	// Create a pattern match constraint for the up-trend's close to exceed a
	// percentage threshold of the immediately preceding downtrend.
	PeriodValueRefPtr closeRef(new ClosePeriodValueRef());

	double depthPercBelowHigh = 100.0 - depthPercRecover_;

	double thresholdValBelowHigh = otherMatch->pointsAtPercentOfDepthBelowHigh(depthPercBelowHigh);
	PatternMatchValidatorPtr recoverPerc(new
				LastValueAbovePointValue(closeRef,thresholdValBelowHigh));

	return recoverPerc;

}

RecoverPercentOfDepth::~RecoverPercentOfDepth() {
}

