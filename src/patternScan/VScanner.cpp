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
#include "PatternMatchFilter.h"
#include "ANDPatternMatchValidator.h"
#include "VPatternMatch.h"

using namespace scannerHelper;

VScanner::VScanner()
{
	trendLineMaxDistancePerc_ = 7.0;
}

VScanner::VScanner(double trendLineMaxDistancePerc)
: trendLineMaxDistancePerc_(trendLineMaxDistancePerc)
{
	assert(trendLineMaxDistancePerc > 0.0);
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE,trendLineMaxDistancePerc_));
	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		logMatchInfo("VScanner: downtrend match",**dtMatchIter);

		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE,trendLineMaxDistancePerc_));
		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

		for(PatternMatchList::iterator utMatchIter = uptrendMatches->begin();
				utMatchIter!=uptrendMatches->end();utMatchIter++)
		{
			PatternMatchValidatorPtr upTrendValidator =
						upTrendValidatorFactory_.createValidator1(*dtMatchIter);
			if(upTrendValidator->validPattern(**utMatchIter))
			{
                PatternMatchPtr overallPattern(new VPatternMatch(*dtMatchIter,*utMatchIter));

				PatternMatchValidatorPtr overallValidate =
						overallValidatorFactory_.createValidator2(*dtMatchIter,*utMatchIter);
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
	return patternMatchFilter::filterUniqueStartEndTime(vMatches);
}


VScanner::~VScanner() {
}

