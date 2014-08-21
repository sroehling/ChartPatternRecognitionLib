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

#define FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND 3
#define UPTREND_MAX_MULTIPLE_DOWNTREND 3
#define FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_UPPER_THRESHOLD 0.35
#define FLAT_BOTTOM_PERCENT_DOWNTREND_DEPTH_LOWER_THRESHOLD 1.4

using namespace scannerHelper;

CupScanner::CupScanner()
{
    trendlineMaxDistancePerc_ = 3.0;

    // Below is the definition of constraints for cup pattern matching. Through experimentation, what seems
    // to work best is to define a multitude of loose contraints. Each (loose) constraint seems to disqualify
    // candidate patterns which are obviously not well-formed. If a pattern doesn't match one constraint, it
    // will likely be caught by another.

    // Another observation about these constraints: If a constraint disqualifies a given candidate pattern
    // match, a shorter one ending no the same date may pass validation and be returned from scanning.
    // After scanning, since pattern matches are filtered by unique end date, then earliest start date,
    // setting constraints may surface different pattern matches in the final results.

    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new SecondPeriodValuePivotsLower()));
    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new HighestHighLessThanFirstHigh()));
    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new LowestLowGreaterThanLastLow()));
    downTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));

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

    upTrendValidatorFactory_.addStaticValidator(patternMatchValidatorCreationHelper::highestHighBelowLastHigh());
    upTrendValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(65.0)));
    upTrendValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
    // TODO - Uptrend needs to validate LowestLowGreaterThanFirstLow and HighestHighLessThanLastHigh

}

PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr cupMatches(new PatternMatchList());

    PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));
    PatternMatchListPtr uniqueDowntrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                downtrendScanner->scanPatternMatches(chartVals));
    PatternMatchValidatorPtr downTrendValidator = downTrendValidatorFactory_.createValidator0();
    PatternMatchListPtr downtrendMatches = PatternMatchValidator::filterMatches(downTrendValidator,uniqueDowntrendMatches);

    BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of downtrend matches: " << downtrendMatches->size();


	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
        UnsignedIntRange flatSegmentLengthRange(3,FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND*(*dtMatchIter)->numPeriods());
        PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal(flatSegmentLengthRange.maxVal());
        TrendLineScanner flatScanner(TrendLineScanner::FLAT_SLOPE_RANGE,trendlineMaxDistancePerc_,flatSegmentLengthRange);
        PatternMatchListPtr uniqueFlatMatches = patternMatchFilter::filterUniqueStartEndTime(flatScanner.scanPatternMatches(valsForFlatScan));
        PatternMatchValidatorPtr flatValidator = flatBottomValidatorFactory_.createValidator1(*dtMatchIter);
        PatternMatchListPtr flatMatches = PatternMatchValidator::filterMatches(flatValidator,uniqueFlatMatches);

        BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of flat matches: " << flatMatches->size();

		for(PatternMatchList::const_iterator ftMatchIter = flatMatches->begin();
				ftMatchIter!=flatMatches->end();ftMatchIter++)
		{
            UnsignedIntRange upTrendSegmentLengthRange(3,UPTREND_MAX_MULTIPLE_DOWNTREND*(*dtMatchIter)->numPeriods());
            PeriodValSegmentPtr valsForUptrendScan = (*ftMatchIter)->trailingValsWithLastVal(upTrendSegmentLengthRange.maxVal());
            TrendLineScanner uptrendScanner(TrendLineScanner::UPTREND_SLOPE_RANGE,trendlineMaxDistancePerc_,upTrendSegmentLengthRange);
            PatternMatchListPtr uniqueUpTrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                        uptrendScanner.scanPatternMatches(valsForUptrendScan));
            PatternMatchValidatorPtr upTrendValidator = upTrendValidatorFactory_.createValidator2(*dtMatchIter,*ftMatchIter);
            PatternMatchListPtr upTrendMatches = PatternMatchValidator::filterMatches(upTrendValidator,uniqueUpTrendMatches);

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

