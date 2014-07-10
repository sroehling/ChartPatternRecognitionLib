/*
 * VScanner.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#include "VScanner.h"
#include "TrendLineScanner.h"
#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"
#include "LastValueAbovePointValue.h"
#include "ClosePeriodValueRef.h"
#include "AnyPatternMatchValidator.h"
#include <boost/log/trivial.hpp>
#include "ScannerHelper.h"
#include "ANDPatternMatchValidator.h"

using namespace scannerHelper;

VScanner::VScanner(double minRHSBelowLHSofVPerc)
: minRHSBelowLHSofVPerc_(minRHSBelowLHSofVPerc)
{
	assert(minRHSBelowLHSofVPerc_ >= 0.0);
	assert(minRHSBelowLHSofVPerc_ <= 100);
}

void VScanner::addUpTrendValidator(const PatternMatchValidatorPtr &upTrendValidator)
{
	customUpTrendValidators_.push_back(upTrendValidator);
}

PatternMatchValidatorPtr VScanner::uptrendPercOfDowntrendValidator(const PatternMatchPtr &downtrendMatch) const
{
	// Create a pattern match constraint for the up-trend's close to exceed a
	// percentage threshold of the immediately preceding downtrend.
	PeriodValueRefPtr closeRef(new ClosePeriodValueRef());
	double thresholdValBelowHigh = downtrendMatch->pointsAtPercentOfDepthBelowHigh(minRHSBelowLHSofVPerc_);
	BOOST_LOG_TRIVIAL(debug) << "VScanner: Threshold for last value (close) of uptrend (last value must exceed): "
			<< thresholdValBelowHigh;
	PatternMatchValidatorPtr uptrendPercOfDownTrend(new
				LastValueAbovePointValue(closeRef,thresholdValBelowHigh));

	return uptrendPercOfDownTrend;
}

PatternMatchValidatorPtr VScanner::upTrendValidator(const PatternMatchPtr &downTrend) const
{
	PatternMatchValidatorPtr uptrendPercOfDownTrend = this->uptrendPercOfDowntrendValidator(downTrend);

	PatternMatchValidatorList upTrendValidators;
	upTrendValidators.push_back(uptrendPercOfDowntrendValidator(downTrend));
	upTrendValidators.insert(upTrendValidators.end(),customUpTrendValidators_.begin(),customUpTrendValidators_.end());

	return PatternMatchValidatorPtr(new ANDPatternMatchValidator(upTrendValidators));
}

PatternMatchValidatorPtr VScanner::overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &upTrend) const
{



	PatternMatchValidatorList finalValidators;
	finalValidators.push_back(PatternMatchValidatorPtr(new AnyPatternMatchValidator()));
	PatternMatchValidatorPtr overallMatchValidator (new ORPatternMatchValidator (finalValidators));
	return overallMatchValidator;
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));
	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		logMatchInfo("VScanner: downtrend match",**dtMatchIter);

		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE));
		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

		for(PatternMatchList::iterator utMatchIter = uptrendMatches->begin();
				utMatchIter!=uptrendMatches->end();utMatchIter++)
		{
			PatternMatchValidatorPtr upTrendValidator = this->upTrendValidator(*dtMatchIter);
			if(upTrendValidator->validPattern(**utMatchIter))
			{
				PatternMatchPtr overallPattern = (*dtMatchIter)->appendMatch(**utMatchIter);

				PatternMatchValidatorPtr overallValidate = this->overallValidator(*dtMatchIter,*utMatchIter);
				if(overallValidate->validPattern(*overallPattern))
				{
					vMatches->push_back(overallPattern);
				}
			}

		} // For each matching up-trend pattern
	} // for each down-trend match

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches: " <<  vMatches->size();


	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	return filterUniqueMatches(vMatches);
}


VScanner::~VScanner() {
}

