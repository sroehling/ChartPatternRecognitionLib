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
#include "ClosePeriodValueRef.h"
#include "AnyPatternMatchValidator.h"
#include <boost/log/trivial.hpp>
#include "ScannerHelper.h"
#include "ANDPatternMatchValidator.h"

using namespace scannerHelper;

VScanner::VScanner()
{
}


void VScanner::addOverallValidator(const PatternMatchValidatorPtr &overallValidator)
{
	customOverallValidators_.push_back(overallValidator);
}


PatternMatchValidatorPtr VScanner::overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &upTrend) const
{
	return PatternMatchValidatorPtr(new ANDPatternMatchValidator(customOverallValidators_));
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));
	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		logMatchInfo("VScanner: downtrend match",**dtMatchIter);

		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE));
		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

		for(PatternMatchList::iterator utMatchIter = uptrendMatches->begin();
				utMatchIter!=uptrendMatches->end();utMatchIter++)
		{
			PatternMatchValidatorPtr upTrendValidator =
						upTrendValidatorFactory_.createValidator(*dtMatchIter);
			if(upTrendValidator->validPattern(**utMatchIter))
			{
				PatternMatchPtr overallPattern = (*dtMatchIter)->appendMatch(**utMatchIter);

				PatternMatchValidatorPtr overallValidate = this->overallValidator(*dtMatchIter,*utMatchIter);
				if(overallValidate->validPattern(*overallPattern))
				{
					vMatches->push_back(overallPattern);
				}
			}

		} // For each matching up-trend pattern
	} // for each down-trend match

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches: " <<  vMatches->size();


	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	return filterUniqueMatches(vMatches);
}


VScanner::~VScanner() {
}

