/*
 * DoubleBottomScanner.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include <DoubleBottomScanner.h>
#include "VScanner.h"
#include "PatternMatch.h"
#include "PatternMatchValueRef.h"
#include "ValueComparisonMatchValidator.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "ScannerHelper.h"
#include "RecoverPercentOfDepth.h"
#include "PatternMatchFilter.h"
#include "DoubleBottomPatternMatch.h"
#include "BreakoutAboveFirstHighValidatorFactory.h"
#include "HighestHighLessThanFirstHigh.h"
#include "PrevPatternValueRatioValidatorFactory.h"
#include "PatternMatchValueRef.h"
#include "PrevPatternValueCompareValidatorFactory.h"

#define DOUBLE_BOTTON_MIN_RHS_VS_LHS_NUM_PERIODS_RATIO 0.33
#define DOUBLE_BOTTON_MAX_RHS_VS_LHS_NUM_PERIODS_RATIO 3.0

#define DOUBLE_BOTTON_MIN_RHS_VS_LHS_DEPTH_RATIO 0.5
#define DOUBLE_BOTTON_MAX_RHS_VS_LHS_DEPTH_RATIO 2.0


void DoubleBottomScanner::initValidators()
{
    // Final high for the LHS of the double bottom must be lower than the first high.
    lhsVValidatorFactory_.addStaticValidator(PatternMatchValidatorPtr(new HighestHighLessThanFirstHigh()));

    // The left and right V should be in proportion to one another (e.g., the left V
    // shouldn't be 5% down while the right V is 25% down).
    DoubleRange validLengthRatios(DOUBLE_BOTTON_MIN_RHS_VS_LHS_NUM_PERIODS_RATIO,
                                  DOUBLE_BOTTON_MAX_RHS_VS_LHS_NUM_PERIODS_RATIO);
    rhsVValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(
        new PrevPatternValueRatioValidatorFactory(validLengthRatios,
                PatternMatchValueRefPtr(new NumPeriodsPatternMatchValueRef()))));

    DoubleRange validDepthRatios(DOUBLE_BOTTON_MIN_RHS_VS_LHS_DEPTH_RATIO,
                                  DOUBLE_BOTTON_MAX_RHS_VS_LHS_DEPTH_RATIO);
    rhsVValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(
        new PrevPatternValueRatioValidatorFactory(validDepthRatios,
                PatternMatchValueRefPtr(new DepthPointsPatternMatchValueRef()))));

    rhsVValidatorFactory_.addFactory(PatternMatchValidatorFactoryPtr(
        new PrevPatternValueCompareValidatorFactory(
            ValueComparatorPtr(new LessThanEqualValueComparator()),
            PatternMatchValueRefPtr(new LowestLowPatternMatchValueRef()))));

  }

DoubleBottomScanner::DoubleBottomScanner(const DoubleRange &minMaxDepthPerc)
: minMaxDepthPerc_(minMaxDepthPerc)
{
	assert(minMaxDepthPerc.positiveVals());
    initValidators();
}

DoubleBottomScanner::DoubleBottomScanner()
: minMaxDepthPerc_(DoubleRange(7.0,30.0))
{
    initValidators();
}

PatternMatchListPtr DoubleBottomScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	using namespace patternMatchValidatorCreationHelper;
	using namespace scannerHelper;

	// The 1st up-trend of the double-bottom must no more than 40% below the depth of the
	// 1st down-trend.
	VScanner leftVScanner;

    // TODO - Something like HighestHighLessThanFirstHigh constaint is needed, but is causing one of the relatively well-formed
    // QCOR test case in the unit tests to fail. More investigation & calibration is needed. Perhaps what is needed is to
    // validate that no typical values exceed the first value, allowing a high wick to go above the start.
    //    leftVScanner.downTrendValidatorFactory().addStaticValidator(PatternMatchValidatorPtr(new HighestHighLessThanFirstHigh));
	leftVScanner.upTrendValidatorFactory().addFactory(
			PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(60.0)));
    PatternMatchValidatorPtr lhsVValidator(lhsVValidatorFactory_.createValidator0());

    PatternMatchListPtr leftVMatches = PatternMatchValidator::filterMatches(lhsVValidator,
                                             leftVScanner.scanPatternMatches(chartVals));

	PatternMatchListPtr dblBottomMatches(new PatternMatchList());

	for(PatternMatchList::const_iterator leftMatchIter = leftVMatches->begin();
			leftMatchIter!=leftVMatches->end();leftMatchIter++)
	{
		PeriodValSegmentPtr valsForRightVScan = (*leftMatchIter)->trailingValsWithLastVal();

        // The RHS of the RHS V must recover at lest 50% of of the LHS of this V to at least
        // represent a partial pattern match. However, for the pattern match to be complete (confirmed),
        // the last close on the RHS must be above the first high of the RHS V..
		VScanner rightVScanner;
        // TODO - Something like HighestHighLessThanFirstHigh constaint is needed, but is causing one of the relatively well-formed
        // QCOR test case in the unit tests to fail. More investigation & calibration is needed.
//        rightVScanner.downTrendValidatorFactory().addStaticValidator(PatternMatchValidatorPtr(new HighestHighLessThanFirstHigh));
        rightVScanner.upTrendValidatorFactory().addFactory(
                PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(50.0)));
        rightVScanner.upTrendValidatorFactory().addFactory(PatternMatchValidatorFactoryPtr(new BreakoutAboveFirstHighValidatorFactory()));

        // Filter down the scanned results for the RHS to those passing the validation constraints.
        PatternMatchValidatorPtr rhsVValidator(rhsVValidatorFactory_.createValidator1(*leftMatchIter));
        PatternMatchListPtr rightVMatches = PatternMatchValidator::filterMatches(rhsVValidator,
                                  rightVScanner.scanPatternMatches(valsForRightVScan));


        // Put the left V and Right V pattern matches together, forming the overall double-bottom pattern.
        PatternMatchListPtr overallMatches(new PatternMatchList());
        for(PatternMatchList::const_iterator rightMatchIter = rightVMatches->begin();
            rightMatchIter != rightVMatches->end(); rightMatchIter++)
        {
            PatternMatchPtr doubleBottomPatternMatch(new DoubleBottomPatternMatch(*leftMatchIter,*rightMatchIter));
            overallMatches->push_back(doubleBottomPatternMatch);
        } // for each "right V" match

		appendValidatedMatches(dblBottomMatches,overallMatches,depthWithinRangeValidator(minMaxDepthPerc_));

	} // for each "left V" match.

	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	return patternMatchFilter::filterUniqueStartEndTime(dblBottomMatches);
}

DoubleBottomScanner::~DoubleBottomScanner() {
}

