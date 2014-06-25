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

CupScanner::CupScanner() {

}


PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(-1.2,-0.5));
	PatternScannerPtr flatScanner(new TrendLineScanner(-0.5,0.5));
	PatternScannerPtr uptrendScanner(new TrendLineScanner(0.5,1.2));

	PatternMatchListPtr cupMatches(new PatternMatchList());

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr flatMatches = flatScanner->scanPatternMatches(valsForFlatScan);
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
			ORPatternMatchValidator plusMinusWithinStart(finalValidators);
			for(PatternMatchList::const_iterator overallIter = overallMatches->begin();
						overallIter != overallMatches->end(); overallIter++)
			{
				if(plusMinusWithinStart.validPattern(**overallIter))
				{
					cupMatches->push_back(*overallIter);
				}

			}

		} // For each combined down trend and flat area match
	} // For each down trend match

	return cupMatches;
}

CupScanner::~CupScanner() {
}

