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

VScanner::VScanner() {

}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(-1.2,-0.5));
	PatternScannerPtr uptrendScanner(new TrendLineScanner(0.5,1.2));

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);

	PatternMatchListPtr vMatches(new PatternMatchList());


	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

		PatternMatchListPtr downUpMatches = (*dtMatchIter)->appendMatchList(*uptrendMatches);

		// Perform a final validation on the pattern as a whole.
		PatternMatchValidatorList finalValidators;
		finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(5.0)));
		finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(-5.0)));
		ORPatternMatchValidator plusMinusWithinStart(finalValidators);
		for(PatternMatchList::const_iterator overallIter = downUpMatches->begin();
					overallIter != downUpMatches->end(); overallIter++)
		{
			if(plusMinusWithinStart.validPattern(**overallIter))
			{
				vMatches->push_back(*overallIter);
			}

		}

	} // for each downtrend match

	return vMatches;
}


VScanner::~VScanner() {
}

