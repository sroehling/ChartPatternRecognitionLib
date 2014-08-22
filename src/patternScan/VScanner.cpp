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
#include "PatternMatchFilter.h"
#include "ANDPatternMatchValidator.h"
#include "VPatternMatch.h"
#include "UnsignedIntRange.h"
#include "SingleSegmentPatternScannerEngine.h"
#include "PatternSlopeWithinRange.h"
#include "PatternSegmentValsCloseToLinearEq.h"
#include "ValuesCloseToTrendlineValidator.h"

using namespace scannerHelper;

#define UP_TREND_MAX_MULTIPLE_DOWNTREND_PERIODS 2

VScanner::VScanner()
{
	trendLineMaxDistancePerc_ = 7.0;

    scannerHelper::populateStandardDowntrendValidationFactories(downTrendValidatorFactory_);
    scannerHelper::populateStandardUpTrendValidationFactories(upTrendValidatorFactory_);
}

VScanner::VScanner(double trendLineMaxDistancePerc)
: trendLineMaxDistancePerc_(trendLineMaxDistancePerc)
{
	assert(trendLineMaxDistancePerc > 0.0);
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE,trendLineMaxDistancePerc_));

    SingleSegmentPatternScannerEngine downTrendScanner;
    downTrendScanner.validatorFactory().addStaticValidator(
                PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::DOWNTREND_SLOPE_RANGE)));
     downTrendScanner.validatorFactory().addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));

    PatternMatchListPtr unfilteredDownTrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                downTrendScanner.scanPatternMatches(chartVals));
    PatternMatchValidatorPtr downTrendValidator = downTrendValidatorFactory_.createValidator0();
    PatternMatchListPtr downtrendMatches = PatternMatchValidator::filterMatches(downTrendValidator,unfilteredDownTrendMatches);

    BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		logMatchInfo("VScanner: downtrend match",**dtMatchIter);

        // Constrain the length of the up-trend (in periods) to twice the down-trend. This ensures the up-trend is
        // relatively well-balance versus the down-trend and also greatly improves scanning performance.
        // TODO - Consider some kind of parameterization of this constraint.
        UnsignedIntRange upTrendSegmentLengthRange(3,UP_TREND_MAX_MULTIPLE_DOWNTREND_PERIODS*(*dtMatchIter)->numPeriods());

        // Only pass in the maximum number of values possibly needed for the up-trend scanning.
        PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal(upTrendSegmentLengthRange.maxVal());

        SingleSegmentPatternScannerEngine upTrendScanner;
        upTrendScanner.validatorFactory().addStaticValidator(
                    PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::UPTREND_SLOPE_RANGE)));
        upTrendScanner.validatorFactory().addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
        PatternMatchListPtr unValidatedUptrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                    upTrendScanner.scanPatternMatches(valsForUptrendScan));
        PatternMatchValidatorPtr upTrendValidator = upTrendValidatorFactory_.createValidator1(*dtMatchIter);
        PatternMatchListPtr upTrendMatches = PatternMatchValidator::filterMatches(upTrendValidator,unValidatedUptrendMatches);

        BOOST_LOG_TRIVIAL(debug) << "VScanner: number of uptrend matches: " << upTrendMatches->size();

        for(PatternMatchList::iterator utMatchIter = upTrendMatches->begin();
                utMatchIter!=upTrendMatches->end();utMatchIter++)
		{
			PatternMatchValidatorPtr upTrendValidator =
						upTrendValidatorFactory_.createValidator1(*dtMatchIter);
			if(upTrendValidator->validPattern(**utMatchIter))
			{
                PatternMatchPtr overallPattern(new VPatternMatch(*dtMatchIter,*utMatchIter));

				PatternMatchValidatorPtr overallValidate =
						overallValidatorFactory_.createValidator2(*dtMatchIter,*utMatchIter);
				if(overallValidate->validPattern(*overallPattern))
				{
					vMatches->push_back(overallPattern);
				}
			}

		} // For each matching up-trend pattern
	} // for each down-trend match

    BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches (unfiltered): " <<  vMatches->size();

    // For purposes of pattern matching, there's no need to return duplicate patterns with
    // the same start and end date.
    PatternMatchListPtr filteredOverallMatches = patternMatchFilter::filterUniqueStartEndTime(vMatches);
    BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches (filtered): " <<  filteredOverallMatches->size();
    logMatchesInfo("VScanner: overall matches (filtered)",filteredOverallMatches);

    return filteredOverallMatches;
}


VScanner::~VScanner() {
}

