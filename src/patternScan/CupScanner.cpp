/*
 * CupScanner.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"
#include "CupScanner.h"
#include "TrendLineScanner.h"
#include "ScannerHelper.h"

using namespace scannerHelper;

CupScanner::CupScanner() {

}


PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));
	PatternScannerPtr flatScanner(new TrendLineScanner(TrendLineScanner::FLAT_SLOPE_RANGE));
	PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE));

	PatternMatchListPtr cupMatches(new PatternMatchList());

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr flatMatches = flatScanner->scanPatternMatches(valsForFlatScan);

		// Append the flat matches to the current downtrend
		PatternMatchListPtr downFlatMatches = (*dtMatchIter)->appendMatchList(*flatMatches);

		for(PatternMatchList::const_iterator downFlatMatchIter = downFlatMatches->begin();
				downFlatMatchIter!=downFlatMatches->end();downFlatMatchIter++)
		{
			PeriodValSegmentPtr valsForUptrendScan = (*downFlatMatchIter)->trailingValsWithLastVal();
			PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

			PatternMatchListPtr overallMatches = (*downFlatMatchIter)->appendMatchList(*uptrendMatches);

			// Perform a final validation on the pattern as a whole.
			PatternMatchValidatorList finalValidators;
			finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(8.0)));
			finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(-3.0)));
			PatternMatchValidatorPtr overallValidator(new ORPatternMatchValidator(finalValidators));

			appendValidatedMatches(cupMatches,overallMatches,overallValidator);

		} // For each combined down trend and flat area match
	} // For each down trend match

	return filterUniqueMatches(cupMatches);
}

