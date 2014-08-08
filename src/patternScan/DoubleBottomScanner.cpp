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

DoubleBottomScanner::DoubleBottomScanner(const DoubleRange &minMaxDepthPerc)
: minMaxDepthPerc_(minMaxDepthPerc)
{
	assert(minMaxDepthPerc.positiveVals());
}

DoubleBottomScanner::DoubleBottomScanner()
: minMaxDepthPerc_(DoubleRange(7.0,30.0))
{
}

PatternMatchListPtr DoubleBottomScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	using namespace patternMatchValidatorCreationHelper;
	using namespace scannerHelper;

	// The 1st up-trend of the double-bottom must no more than 40% below the depth of the
	// 1st down-trend.
	VScanner leftVScanner;
	leftVScanner.upTrendValidatorFactory().addFactory(
			PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(60.0)));
	PatternMatchListPtr leftVMatches = leftVScanner.scanPatternMatches(chartVals);

	PatternMatchListPtr dblBottomMatches(new PatternMatchList());

	for(PatternMatchList::const_iterator leftMatchIter = leftVMatches->begin();
			leftMatchIter!=leftVMatches->end();leftMatchIter++)
	{
		PeriodValSegmentPtr valsForRightVScan = (*leftMatchIter)->trailingValsWithLastVal();

		// The RHS of the RHS V must recover 100% of of the LHS of this V. In other words,
		// for the pattern match to be complete (confirmed), the right-most up-trend of the
		// double-bottom must exceed the start of the 2nd down-trend of the double-bottom.
		VScanner rightVScanner;
		rightVScanner.upTrendValidatorFactory().addFactory(
				PatternMatchValidatorFactoryPtr(new RecoverPercentOfDepth(100.0)));

		// Filter down the scanned results for the RHS to those RHS V's which have a lower low
		// than the LHS.
		PatternMatchValidatorPtr rhsLowerLowValid = lowerLowValidator(*leftMatchIter);
		PatternMatchListPtr rightVMatches = PatternMatchValidator::filterMatches(rhsLowerLowValid,
				rightVScanner.scanPatternMatches(valsForRightVScan));

		// TODO - The left and right V should be in proportion to one another (e.g., the left V
		// shouldn't be 5% down while the right V is 25% down).
		// Put the left V and Right V pattern matches together, forming the overall double-bottom pattern.
        PatternMatchListPtr overallMatches(new PatternMatchList());
        for(PatternMatchList::const_iterator rightMatchIter = rightVMatches->begin();
            rightMatchIter != rightVMatches->end(); rightMatchIter++)
        {
            overallMatches->push_back(PatternMatchPtr(new DoubleBottomPatternMatch(*leftMatchIter,*rightMatchIter)));
        } // for each "right V" match

		appendValidatedMatches(dblBottomMatches,overallMatches,depthWithinRangeValidator(minMaxDepthPerc_));

	} // for each "left V" match.

	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	return patternMatchFilter::filterUniqueStartEndTime(dblBottomMatches);
}

DoubleBottomScanner::~DoubleBottomScanner() {
}

