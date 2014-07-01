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

VScanner::VScanner(double minUptrendPercentOfDowntrend)
: minUptrendPercentOfDowntrend_(minUptrendPercentOfDowntrend)
{
	assert(minUptrendPercentOfDowntrend_ >= 0.0);
	assert(minUptrendPercentOfDowntrend_ <= 100);
}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner(new TrendLineScanner(-3.0,-0.5));

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of downtrend matches: " << downtrendMatches->size();

	PatternMatchListPtr vMatches(new PatternMatchList());

	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
		PeriodValSegmentPtr valsForUptrendScan = (*dtMatchIter)->trailingValsWithLastVal();
		BOOST_LOG_TRIVIAL(debug) << "VScanner: downtrend match: " << (**dtMatchIter);

		// Create a pattern match constraint for the up-trend's close to exceed a
		// percentage threshold of the immediately preceding downtrend.
		PeriodValueRefPtr closeRef(new ClosePeriodValueRef());
		double thresholdValBelowHigh = (*dtMatchIter)->pointsAtPercentOfDepthBelowHigh(minUptrendPercentOfDowntrend_);
		PatternMatchValidatorPtr uptrendPercOfDownTrend(new
					LastValueAbovePointValue(closeRef,thresholdValBelowHigh));

		PatternScannerPtr uptrendScanner(new TrendLineScanner(0.5,3.0,uptrendPercOfDownTrend));

		PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);
		BOOST_LOG_TRIVIAL(debug) << "VScanner: number of uptrend matches: " << uptrendMatches->size();

		PatternMatchListPtr downUpMatches = (*dtMatchIter)->appendMatchList(*uptrendMatches);
		BOOST_LOG_TRIVIAL(debug) << "VScanner: down then uptrend matches: " << downUpMatches->size();

		// Perform a final validation on the pattern as a whole.
		PatternMatchValidatorList finalValidators;
		finalValidators.push_back(PatternMatchValidatorPtr(new AnyPatternMatchValidator()));
		ORPatternMatchValidator overallMatchValidator(finalValidators);
		for(PatternMatchList::const_iterator overallIter = downUpMatches->begin();
					overallIter != downUpMatches->end(); overallIter++)
		{
			BOOST_LOG_TRIVIAL(debug) << "VScanner: down and up trend match: " << (**overallIter);
			if(overallMatchValidator.validPattern(**overallIter))
			{
				vMatches->push_back(*overallIter);
			}

		}

	} // for each downtrend match

	BOOST_LOG_TRIVIAL(debug) << "VScanner: number of overall matches: " <<  vMatches->size();

	return vMatches;
}


VScanner::~VScanner() {
}

