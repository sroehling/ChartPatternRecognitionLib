#include "CupWithHandleScanner.h"
#include "CupScanner.h"
#include "CupWithHandlePatternMatch.h"
#include <boost/log/trivial.hpp>
#include "VScanner.h"

#include "SingleSegmentPatternScannerEngine.h"
#include "PatternSlopeWithinRange.h"
#include "TrendLineScanner.h"
#include "BreakoutAboveFirstHighValidatorFactory.h"
#include "CloseWithinPercentOfDepthFromFirstHighValidator.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "PrevPatternValueRatioValidatorFactory.h"
#include "ScannerHelper.h"


#define DEFAULT_CUP_WITH_HANDLE_SCANNER_MIN_SEGMENT_LENGTH 2
#define DEFAULT_CUP_WITH_HANDLE_SCANNER_HANDLE_MAX_PERC_DISTANCE_TRENDLINE 3.0
#define CUP_WITH_HANDLE_SCANNER_CUP_LAST_CLOSE_PERC_ABOVE_DEPTH_FROM_FIRST_CLOSE_THRESHOLD 0.2
#define CUP_WITH_HANDLE_SCANNER_MAX_DEPTH_HANDLE_LHS_CUP 0.5

#define CUP_WITH_HANDLE_MIN_HANDLE_VS_CUP_DEPTH_RATIO 0.15
#define CUP_WITH_HANDLE_MAX_HANDLE_VS_CUP_DEPTH_RATIO 0.65

#define CUP_WITH_HANDLE_MIN_DEPTH_PERC_SINCE_START 8.0
#define CUP_WITH_HANDLE_MAX_DEPTH_PERC_SINCE_START 30.0

using namespace scannerHelper;
using namespace patternMatchValidatorCreationHelper;

CupWithHandleScanner::CupWithHandleScanner()
{
    DoubleRange validDepthRatios(CUP_WITH_HANDLE_MIN_HANDLE_VS_CUP_DEPTH_RATIO,
                                  CUP_WITH_HANDLE_MAX_HANDLE_VS_CUP_DEPTH_RATIO);
    handleValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(
        new PrevPatternValueRatioValidatorFactory(validDepthRatios,
                PatternMatchValueRefPtr(new DepthPointsFirstHighLowestLowPatternMatchValueRef()))));
}


PatternMatchListPtr CupWithHandleScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
    PatternMatchListPtr cupWithHandleMatches(new PatternMatchList());

    CupScanner lhsCupScanner;

    // TODO - Evaluate whether this is necessary. Using the CloseWithinPercentOfDepthFromFirstHighValidator constraint
    // with CupWithoutHandleScanner caused the validation code not match certain patterns.
    lhsCupScanner.overallValidatorFactory().addStaticValidator(PatternMatchValidatorPtr(new CloseWithinPercentOfDepthFromFirstHighValidator(
            CUP_WITH_HANDLE_SCANNER_CUP_LAST_CLOSE_PERC_ABOVE_DEPTH_FROM_FIRST_CLOSE_THRESHOLD)));
    lhsCupScanner.overallValidatorFactory().addStaticValidator(depthSinceStartWithinRangeValidator(
        DoubleRange(CUP_WITH_HANDLE_MIN_DEPTH_PERC_SINCE_START,CUP_WITH_HANDLE_MAX_DEPTH_PERC_SINCE_START)));

    PatternMatchListPtr cupMatches = lhsCupScanner.scanPatternMatches(chartVals);

    BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of cup matches: " << cupMatches->size();


    for(PatternMatchList::const_iterator cupMatchIter = cupMatches->begin();
            cupMatchIter!=cupMatches->end();cupMatchIter++)
    {

        logMatchInfo("CupWithHandleScanner: cup match",**cupMatchIter);

        PeriodValSegmentPtr valsForHandleScan = (*cupMatchIter)->trailingValsWithLastVal();

        unsigned int minHandleSegmentLength = DEFAULT_CUP_WITH_HANDLE_SCANNER_MIN_SEGMENT_LENGTH;
        bool validateWithTrendLineValidator = false;

        CupScanner uShapedHandleScanner(minHandleSegmentLength,validateWithTrendLineValidator);
        uShapedHandleScanner.overallValidatorFactory().addFactory(
                    PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));
        uShapedHandleScanner.overallValidatorFactory().addStaticValidator(
                    patternMatchValidatorCreationHelper::lowAbovePercDepthOfOtherPattern(*cupMatchIter,
                    CUP_WITH_HANDLE_SCANNER_MAX_DEPTH_HANDLE_LHS_CUP));
        PatternMatchValidatorPtr handleValidator = handleValidatorFactory_.createValidator1(*cupMatchIter);

        PatternMatchListPtr uShapedHandleMatches = PatternMatchValidator::filterMatches(handleValidator,
                                           uShapedHandleScanner.scanPatternMatches(valsForHandleScan));
        BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of U shaped handle matches: " << uShapedHandleMatches->size();

        for(PatternMatchList::const_iterator uShapedHandleMatchIter = uShapedHandleMatches->begin();
            uShapedHandleMatchIter != uShapedHandleMatches->end(); uShapedHandleMatchIter++)
        {
            logMatchInfo("CupWithHandleScanner: u shaped handle match",**uShapedHandleMatchIter);

            cupWithHandleMatches->push_back(
                        PatternMatchPtr(new CupWithHandlePatternMatch(*cupMatchIter,*uShapedHandleMatchIter)));
        } // for each u-shaped handle match


        VScanner vShapedHandleScanner(DEFAULT_CUP_WITH_HANDLE_SCANNER_HANDLE_MAX_PERC_DISTANCE_TRENDLINE,
                                      minHandleSegmentLength,validateWithTrendLineValidator);
        vShapedHandleScanner.overallValidatorFactory().addFactory(PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));
        vShapedHandleScanner.overallValidatorFactory().addStaticValidator(
                    patternMatchValidatorCreationHelper::lowAbovePercDepthOfOtherPattern(*cupMatchIter,
                    CUP_WITH_HANDLE_SCANNER_MAX_DEPTH_HANDLE_LHS_CUP));
        PatternMatchListPtr vShapedHandleMatches = PatternMatchValidator::filterMatches(handleValidator,
                                            vShapedHandleScanner.scanPatternMatches(valsForHandleScan));
        BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of V shaped handle matches: " << vShapedHandleMatches->size();

        for(PatternMatchList::const_iterator vShapedHandleMatchIter = vShapedHandleMatches->begin();
            vShapedHandleMatchIter != vShapedHandleMatches->end(); vShapedHandleMatchIter++)
        {
            logMatchInfo("CupWithHandleScanner: v shaped handle match",**vShapedHandleMatchIter);
            cupWithHandleMatches->push_back(
                        PatternMatchPtr(new CupWithHandlePatternMatch(*cupMatchIter,*vShapedHandleMatchIter)));
        } // for each v-shaped handle match

    }

    BOOST_LOG_TRIVIAL(debug) << "CupWithHandleScanner: number of cup with handle matches: " << cupWithHandleMatches->size();

    return cupWithHandleMatches;
}
