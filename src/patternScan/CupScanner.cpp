/*
 * CupScanner.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#include <boost/log/trivial.hpp>


#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"
#include "CupScanner.h"
#include "TrendLineScanner.h"
#include "ScannerHelper.h"
#include "PatternMatchFilter.h"
#include "CupPatternMatch.h"
#include "UnsignedIntRange.h"

#define FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND 3
#define UPTREND_MAX_MULTIPLE_DOWNTREND 2

using namespace scannerHelper;

CupScanner::CupScanner()
{
    trendlineMaxDistancePerc_ = 7.0;
}

PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(TrendLineScanner::DOWNTREND_SLOPE_RANGE));

	PatternMatchListPtr cupMatches(new PatternMatchList());

    PatternMatchListPtr downtrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                downtrendScanner->scanPatternMatches(chartVals));
    BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of downtrend matches: " << downtrendMatches->size();


	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal();

        UnsignedIntRange flatSegmentLengthRange(3,FLAT_BOTTOM_MAX_MULTIPLE_DOWNTREND*(*dtMatchIter)->numPeriods());


        TrendLineScanner flatScanner(TrendLineScanner::FLAT_SLOPE_RANGE,trendlineMaxDistancePerc_,flatSegmentLengthRange);

        PatternMatchListPtr flatMatches = patternMatchFilter::filterUniqueStartEndTime(flatScanner.scanPatternMatches(valsForFlatScan));
        BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of flat matches: " << flatMatches->size();

		for(PatternMatchList::const_iterator ftMatchIter = flatMatches->begin();
				ftMatchIter!=flatMatches->end();ftMatchIter++)
		{
			PeriodValSegmentPtr valsForUptrendScan = (*ftMatchIter)->trailingValsWithLastVal();
            UnsignedIntRange upTrendSegmentLengthRange(3,UPTREND_MAX_MULTIPLE_DOWNTREND*(*dtMatchIter)->numPeriods());
            TrendLineScanner uptrendScanner(TrendLineScanner::UPTREND_SLOPE_RANGE,trendlineMaxDistancePerc_,upTrendSegmentLengthRange);
            PatternMatchListPtr upTrendMatches = patternMatchFilter::filterUniqueStartEndTime(
                        uptrendScanner.scanPatternMatches(valsForUptrendScan));
            BOOST_LOG_TRIVIAL(debug) << "CupScanner: number of uptrend matches: " << upTrendMatches->size();

			PatternMatchValidatorPtr uptrendValidator =
					upTrendValidatorFactory_.createValidator2(*dtMatchIter,*ftMatchIter);

			for(PatternMatchList::const_iterator utMatchIter = upTrendMatches->begin();
					utMatchIter != upTrendMatches->end(); utMatchIter++)
			{
					if(uptrendValidator->validPattern(**utMatchIter))
					{
						PatternMatchValidatorPtr overallValidator = overallValidatorFactory_.createValidator3(
								*dtMatchIter,*ftMatchIter,*utMatchIter);
                        PatternMatchPtr overallMatch(new CupPatternMatch(*dtMatchIter,*ftMatchIter,*utMatchIter));
						if(overallValidator->validPattern(*overallMatch))
						{
							cupMatches->push_back(overallMatch);
						}
					}
			} // for each up trend
		} // for each flat trend
	} // for each down trend

	return patternMatchFilter::filterUniqueStartEndTime(cupMatches);
}

