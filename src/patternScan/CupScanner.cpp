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

CupScanner::CupScanner()
{
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

		for(PatternMatchList::const_iterator ftMatchIter = flatMatches->begin();
				ftMatchIter!=flatMatches->end();ftMatchIter++)
		{
			PeriodValSegmentPtr valsForUptrendScan = (*ftMatchIter)->trailingValsWithLastVal();
			PatternMatchListPtr upTrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

			PatternMatchValidatorPtr uptrendValidator =
					upTrendValidatorFactory_.createValidator2(*dtMatchIter,*ftMatchIter);

			for(PatternMatchList::const_iterator utMatchIter = upTrendMatches->begin();
					utMatchIter != upTrendMatches->end(); utMatchIter++)
			{
					if(uptrendValidator->validPattern(**utMatchIter))
					{
						PatternMatchValidatorPtr overallValidator = overallValidatorFactory_.createValidator3(
								*dtMatchIter,*ftMatchIter,*utMatchIter);
						PatternMatchPtr overallMatch =
									((*dtMatchIter)->appendMatch(**ftMatchIter))->appendMatch(**utMatchIter);
						if(overallValidator->validPattern(*overallMatch))
						{
							cupMatches->push_back(overallMatch);
						}
					}
			} // for each up trend
		} // for each flat trend
	} // for each down trend

	return filterUniqueMatches(cupMatches);
}

