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
#include "LowestLowGreaterThanLastLow.h"
#include "PatternMatchValidatorCreationHelper.h"

using namespace scannerHelper;
using namespace patternMatchValidatorCreationHelper;

#define UP_TREND_MAX_MULTIPLE_DOWNTREND_PERIODS 2
#define DEFAULT_V_SCANNER_MIN_SEGMENT_LENGTH 3
#define DEFAULT_V_SCANNER_MAX_SEGMENT_LENGTH 200
#define DEFAULT_V_SCANNER_MAX_PERC_DISTANCE_TRENDLINE 7.0

void VScanner::initValidationConstraints()
{
    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new LowestLowGreaterThanLastLow()));
    downTrendValidatorFactory_.addStaticValidator(highestCloseBelowFirstHigh());

    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
    upTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
}

VScanner::VScanner()
{
    trendLineMaxDistancePerc_ = DEFAULT_V_SCANNER_MAX_PERC_DISTANCE_TRENDLINE;
    minTrendLineSegmentLength_ = DEFAULT_V_SCANNER_MIN_SEGMENT_LENGTH;

    initValidationConstraints();
}

VScanner::VScanner(double trendLineMaxDistancePerc, unsigned int minTrendLineSegmentLength)
: trendLineMaxDistancePerc_(trendLineMaxDistancePerc),
  minTrendLineSegmentLength_(minTrendLineSegmentLength)
{
	assert(trendLineMaxDistancePerc > 0.0);
    assert(minTrendLineSegmentLength > 1);
    assert(minTrendLineSegmentLength <= 5);

    initValidationConstraints();
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    UnsignedIntRange downTrendSegmentLengthRange(minTrendLineSegmentLength_,DEFAULT_V_SCANNER_MAX_SEGMENT_LENGTH);
    SingleSegmentPatternScannerEngine downTrendScanner(downTrendSegmentLengthRange);
    downTrendScanner.validatorFactory().addStaticValidator(
                PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::DOWNTREND_SLOPE_RANGE)));

    PatternMatchValidatorPtr downTrendValidator = downTrendValidatorFactory_.createValidator0();
    PatternMatchListPtr validatedDownTrendMatches = PatternMatchValidator::filterMatches(downTrendValidator,downTrendScanner.scanPatternMatches(chartVals));
    PatternMatchListPtr downtrendMatches = patternMatchFilter::filterUniqueStartEndTime(validatedDownTrendMatches);

    BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		logMatchInfo("VScanner: downtrend match",**dtMatchIter);

        // Constrain the length of the up-trend (in periods) to twice the down-trend. This ensures the up-trend is
        // relatively well-balance versus the down-trend and also greatly improves scanning performance.
        // TODO - Consider some kind of parameterization of this constraint.
        UnsignedIntRange upTrendSegmentLengthRange(minTrendLineSegmentLength_,UP_TREND_MAX_MULTIPLE_DOWNTREND_PERIODS*(*dtMatchIter)->numPeriods());

        // Only pass in the maximum number of values possibly needed for the up-trend scanning.
        PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal(upTrendSegmentLengthRange.maxVal());

        SingleSegmentPatternScannerEngine upTrendScanner(upTrendSegmentLengthRange);
        upTrendScanner.validatorFactory().addStaticValidator(
                    PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::UPTREND_SLOPE_RANGE)));

        PatternMatchValidatorPtr upTrendValidator = upTrendValidatorFactory_.createValidator1(*dtMatchIter);
        PatternMatchListPtr validatedUpTrendMatches = PatternMatchValidator::filterMatches(upTrendValidator,
                                                                        upTrendScanner.scanPatternMatches(valsForUptrendScan));
        PatternMatchListPtr upTrendMatches = patternMatchFilter::filterUniqueStartEndTime(validatedUpTrendMatches);

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

