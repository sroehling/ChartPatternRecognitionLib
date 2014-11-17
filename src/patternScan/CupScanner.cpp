/*
 * CupScanner.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>


#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"
#include "CupScanner.h"
#include "TrendLineScanner.h"
#include "ScannerHelper.h"
#include "PatternMatchFilter.h"
#include "CupPatternMatch.h"
#include "StaticPatternMatchValidatorFactory.h"
#include "SecondPeriodValuePivotsLower.h"
#include "HighestHighLessThanFirstHigh.h"
#include "LowerHighPatternMatchValidatorFactory.h"
#include "UnsignedIntRange.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "RecoverPercentOfDepth.h"
#include "LowestLowGreaterThanLastLow.h"
#include "PrevPatternDepthThreshold.h"
#include "ValuesCloseToTrendlineValidator.h"
#include "SingleSegmentPatternScannerEngine.h"
#include "PatternSlopeWithinRange.h"
#include "PercentIntersectingPatternLineValidator.h"
#include "PrevPatternValueRatioValidatorFactory.h"

#define FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND 3
#define UPTREND_MAX_MULTIPLE_DOWNTREND 3
#define UPTREND_MAX_MULTIPLE_FLAT_TREND 3
#define FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_UPPER_THRESHOLD 0.35
#define FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_LOWER_THRESHOLD 1.4
#define DEFAULT_CUP_SCANNER_MIN_SEGMENT_LENGTH 3
#define DEFAULT_CUP_SCANNER_MAX_SEGMENT_LENGTH 200
#define DEFAULT_CUP_SCANNER_MAX_PERC_TRENDLINE_FIT 3.0

// The number of periods in the cup bottom must be a little more than 1/2 the number
// of periods in the initial down-trend. So, for example, if the downtrend is 6 periods
// the cup bottom must have at least 4 periods. Otherwise, the cup will look more V like
// than cup like.There is really no maximum bottom length, but 20 should suffice.
#define CUP_MIN_BOTTOM_VS_DOWNTREND_LENGTH_RATIO 0.55
#define CUP_MAX_BOTTOM_VS_DOWNTREND_LENGTH_RATIO 20.0

using namespace scannerHelper;
using namespace patternMatchValidatorCreationHelper;

void CupScanner::initConstraints()
{
    // Below is the definition of constraints for cup pattern matching. Through experimentation, what seems
    // to work best is to define a multitude of loose contraints. Each (loose) constraint seems to disqualify
    // candidate patterns which are obviously not well-formed. If a pattern doesn't match one constraint, it
    // will likely be caught by another.

    // Another observation about these constraints: If a constraint disqualifies a given candidate pattern
    // match, a shorter one ending no the same date may pass validation and be returned from scanning.
    // After scanning, since pattern matches are filtered by unique end date, then earliest start date,
    // setting constraints may surface different pattern matches in the final results.
     downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
     downTrendValidatorFactory_.addStaticValidator(highestCloseBelowFirstHigh());

    flatBottomValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(new LowerHighPatternMatchValidatorFactory()));

    // Flat area should not go above FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_UPPER_THRESHOLD
    // of the depth of the initial down-trend. Similarly, no values in the
    // flat area should go below FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_LOWER_THRESHOLD of the depth.
    flatBottomValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(new PrevPatternDepthThreshold(
                  FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_UPPER_THRESHOLD,PatternMatchValueRefPtr(new HighestHighPatternMatchValueRef()),
                     ValueComparatorPtr(new LessThanEqualValueComparator()))));
    flatBottomValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(new PrevPatternDepthThreshold(
                  FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_LOWER_THRESHOLD,PatternMatchValueRefPtr(new LowestLowPatternMatchValueRef()),
                     ValueComparatorPtr(new GreaterThanEqualValueComparator()))));

    // TODO - Consider OR'ing the constraint below with one that simply verifies the length is at least
    // 4. If the length is at least 4, no matter how long the downtrend is, the cup should be reasonably
    // well formed.
    DoubleRange validFlatBottomVsDowntrendRatios(CUP_MIN_BOTTOM_VS_DOWNTREND_LENGTH_RATIO,
                                  CUP_MAX_BOTTOM_VS_DOWNTREND_LENGTH_RATIO);
    flatBottomValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(
        new PrevPatternValueRatioValidatorFactory(validFlatBottomVsDowntrendRatios,
                PatternMatchValueRefPtr(new NumPeriodsPatternMatchValueRef()))));


    flatBottomValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new PercentIntersectingPatternLineValidator()));

    upTrendValidatorFactory_.addStaticValidator(patternMatchValidatorCreationHelper::highestHighBelowLastHigh());
    upTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));

    upTrendValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(65.0)));

}

CupScanner::CupScanner()
{
    trendlineMaxDistancePerc_ = DEFAULT_CUP_SCANNER_MAX_PERC_TRENDLINE_FIT;
    minTrendLineSegmentLength_ = DEFAULT_CUP_SCANNER_MIN_SEGMENT_LENGTH;
    initConstraints();
}

CupScanner::CupScanner(unsigned int minTrendLineSegmentLength)
    : minTrendLineSegmentLength_(minTrendLineSegmentLength)
{
    assert(minTrendLineSegmentLength > 1); // Segment length <= 1 doesn't make sense
    assert(minTrendLineSegmentLength < 5); // constraint to something sensible

    trendlineMaxDistancePerc_ = DEFAULT_CUP_SCANNER_MAX_PERC_TRENDLINE_FIT;
    initConstraints();
}

PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr cupMatches(new PatternMatchList());

    // TODO - Need to filter based upon the constraints first, then the unique start & end time
    SingleSegmentPatternScannerEngine downTrendScanner(UnsignedIntRange(minTrendLineSegmentLength_,DEFAULT_CUP_SCANNER_MAX_SEGMENT_LENGTH));
    downTrendScanner.validatorFactory().addStaticValidator(
                PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::DOWNTREND_SLOPE_RANGE)));

    PatternMatchValidatorPtr downTrendValidator = downTrendValidatorFactory_.createValidator0();
    PatternMatchListPtr validatedDowntrendMatches = PatternMatchValidator::filterMatches(downTrendValidator,
                       downTrendScanner.scanPatternMatches(chartVals));
    PatternMatchListPtr downtrendMatches = patternMatchFilter::filterUniqueStartEndTime(validatedDowntrendMatches);

    BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of downtrend matches: " << downtrendMatches->size();


	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{

        UnsignedIntRange flatSegmentLengthRange(minTrendLineSegmentLength_,FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND*(*dtMatchIter)->numPeriods());
        PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal(flatSegmentLengthRange.maxVal());

        SingleSegmentPatternScannerEngine flatScanner(flatSegmentLengthRange);
        flatScanner.validatorFactory().addStaticValidator(
                    PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::FLAT_SLOPE_RANGE)));
        PatternMatchValidatorPtr flatValidator = flatBottomValidatorFactory_.createValidator1(*dtMatchIter);
        PatternMatchListPtr validatedFlatMatches = PatternMatchValidator::filterMatches(flatValidator,flatScanner.scanPatternMatches(valsForFlatScan));
        PatternMatchListPtr flatMatches = patternMatchFilter::filterUniqueStartEndTime(validatedFlatMatches);

        BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of flat matches: " << flatMatches->size();

		for(PatternMatchList::const_iterator ftMatchIter = flatMatches->begin();
				ftMatchIter!=flatMatches->end();ftMatchIter++)
		{

            // Impose some constraints on the number of periods in the uptrend versus both the
            // preceding downtrend and flat-area/trend. If the uptrend is much longer than either the
            // preceding downtrend or flat area, the cup will have a more "V like" appearance.
            unsigned int maxVsFlatTrend = UPTREND_MAX_MULTIPLE_FLAT_TREND * (*ftMatchIter)->numPeriods();
            unsigned int maxVsDownTrend = UPTREND_MAX_MULTIPLE_DOWNTREND * (*dtMatchIter)->numPeriods();
            unsigned int overallUpTrendMaxPeriods = std::min(maxVsFlatTrend,maxVsDownTrend);
            assert(overallUpTrendMaxPeriods >= minTrendLineSegmentLength_);
            BOOST_LOG_TRIVIAL(debug) << "CupScanner: max uptrend periods:  " << overallUpTrendMaxPeriods;

            UnsignedIntRange upTrendSegmentLengthRange(minTrendLineSegmentLength_,overallUpTrendMaxPeriods);
            PeriodValSegmentPtr valsForUptrendScan = (*ftMatchIter)->trailingValsWithLastVal(upTrendSegmentLengthRange.maxVal());

            SingleSegmentPatternScannerEngine upTrendScanner(upTrendSegmentLengthRange);
            upTrendScanner.validatorFactory().addStaticValidator(
                        PatternMatchValidatorPtr(new PatternSlopeWithinRange(TrendLineScanner::UPTREND_SLOPE_RANGE)));

            PatternMatchValidatorPtr upTrendValidator = upTrendValidatorFactory_.createValidator2(*dtMatchIter,*ftMatchIter);
            PatternMatchListPtr validatedUpTrendMatches = PatternMatchValidator::filterMatches(upTrendValidator,
                                  upTrendScanner.scanPatternMatches(valsForUptrendScan));
            PatternMatchListPtr upTrendMatches = patternMatchFilter::filterUniqueStartEndTime(validatedUpTrendMatches);

            BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of uptrend matches: " << upTrendMatches->size();

			PatternMatchValidatorPtr uptrendValidator =
					upTrendValidatorFactory_.createValidator2(*dtMatchIter,*ftMatchIter);

			for(PatternMatchList::const_iterator utMatchIter = upTrendMatches->begin();
					utMatchIter != upTrendMatches->end(); utMatchIter++)
			{
					if(uptrendValidator->validPattern(**utMatchIter))
					{
						PatternMatchValidatorPtr overallValidator = overallValidatorFactory_.createValidator3(
								*dtMatchIter,*ftMatchIter,*utMatchIter);
                        PatternMatchPtr overallMatch(new CupPatternMatch(*dtMatchIter,*ftMatchIter,*utMatchIter));
						if(overallValidator->validPattern(*overallMatch))
						{
							cupMatches->push_back(overallMatch);
						}
					}
			} // for each up trend
		} // for each flat trend
	} // for each down trend

	return patternMatchFilter::filterUniqueStartEndTime(cupMatches);
}

