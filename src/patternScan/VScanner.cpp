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
#include "FilterUniqueStartEndDate.h"

VScanner::VScanner(double minRHSBelowLHSofVPerc)
: minRHSBelowLHSofVPerc_(minRHSBelowLHSofVPerc)
{
	assert(minRHSBelowLHSofVPerc_ >= 0.0);
	assert(minRHSBelowLHSofVPerc_ <= 100);
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

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		ScannerHelper::logMatchInfo("VScanner: downtrend match",**dtMatchIter);

		PatternMatchValidatorPtr uptrendPercOfDownTrend = uptrendPercOfDowntrendValidator(*dtMatchIter);

		PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE,uptrendPercOfDownTrend));

		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);


		BOOST_LOG_TRIVIAL(debug) << "VScanner: number of uptrend matches: " << uptrendMatches->size();
		PatternMatchListPtr downUpMatches = (*dtMatchIter)->appendMatchList(*uptrendMatches);

		// Perform a final validation on the pattern as a whole.
		PatternMatchValidatorList finalValidators;
		finalValidators.push_back(PatternMatchValidatorPtr(new AnyPatternMatchValidator()));
		ORPatternMatchValidator overallMatchValidator(finalValidators);
		for(PatternMatchList::const_iterator overallIter = downUpMatches->begin();
					overallIter != downUpMatches->end(); overallIter++)
		{
			if(overallMatchValidator.validPattern(**overallIter))
			{
				ScannerHelper::logMatchInfo("VScanner: down and up trend match",**overallIter);
				vMatches->push_back(*overallIter);
			}

		}

	} // for each downtrend match

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches: " <<  vMatches->size();


	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	FilterUniqueStartEndDate uniqueStartEndDateFilter;
	PatternMatchListPtr uniqueMatches = uniqueStartEndDateFilter.filterPatternMatches(vMatches);
	return uniqueMatches;
}


VScanner::~VScanner() {
}

