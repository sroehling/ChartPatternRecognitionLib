/*
 * InvertedVScanner.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <InvertedVScanner.h>

#include "TrendLineScanner.h"
#include <boost/log/trivial.hpp>
#include "ScannerHelper.h"
#include "PatternMatchFilter.h"

using namespace scannerHelper;


InvertedVScanner::InvertedVScanner() {
}

PatternMatchListPtr InvertedVScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr uptrendScanner(new TrendLineScanner(TrendLineScanner::UPTREND_SLOPE_RANGE));
	PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(chartVals);
	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of up-trend matches: " << uptrendMatches->size();

	PatternMatchListPtr invertedVMatches(new PatternMatchList());

	for(PatternMatchList::iterator utMatchIter = uptrendMatches->begin();
			utMatchIter!=uptrendMatches->end();utMatchIter++)
	{
		logMatchInfo("InvertedVScanner: up-trend match",**utMatchIter);

		PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));
		PeriodValSegmentPtr valsForDowntrendScan = (*utMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(valsForDowntrendScan);

		for(PatternMatchList::iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
		{
			PatternMatchValidatorPtr downTrendValidator =
						downTrendValidatorFactory_.createValidator1(*dtMatchIter);
			if(downTrendValidator->validPattern(**dtMatchIter))
			{
				PatternMatchPtr overallPattern = (*utMatchIter)->appendMatch(**dtMatchIter);

				PatternMatchValidatorPtr overallValidate =
						overallValidatorFactory_.createValidator2(*utMatchIter,*dtMatchIter);
				if(overallValidate->validPattern(*overallPattern))
				{
					invertedVMatches->push_back(overallPattern);
				}
			}

		} // For each matching up-trend pattern
	} // for each down-trend match

	BOOST_LOG_TRIVIAL(debug) << "InvertedVScanner: number of overall matches: " <<  invertedVMatches->size();


	// For purposes of pattern matching, there's no need to return duplicate patterns with
	// the same start and end date.
	return patternMatchFilter::filterUniqueStartEndTime(invertedVMatches);
}


InvertedVScanner::~InvertedVScanner() {
}

