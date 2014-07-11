/*
 * AscendingBasePullbackScanner.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#include <AscendingBasePullbackScanner.h>
#include "CupScanner.h"
#include "CupScannerConfigurator.h"
#include "VScanner.h"
#include "TrendLineScanner.h"
#include "ScannerHelper.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "DoubleRange.h"

using namespace patternMatchValidatorCreationHelper;
using namespace scannerHelper;

AscendingBasePullbackScanner::AscendingBasePullbackScanner(const DoubleRange &minMaxDepthPerc)
: minMaxDepthPerc_(minMaxDepthPerc)
{
}

PatternMatchListPtr AscendingBasePullbackScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr pullbackMatches(new PatternMatchList());

	// TODO - Need to measure depth from the start of the pattern, rather than the overall depth.
	// This is because the last uptrend in the pattern is expected to extend beyond the starting
	// high.
	PatternMatchValidatorPtr overallDepthValidator = depthWithinRangeValidator(minMaxDepthPerc_);

	// An optional short flat area leading into the pull-back; i.e., the price will
	// optionally "roll over" into a pull-back instead of immediately correcting.
	// TODO - Configure rolloverScanner to have a narrower "curve fit" than default
	TrendLineScanner rolloverScanner(TrendLineScanner::FLAT_SLOPE_RANGE);
	PatternMatchListPtr rolloverMatches = rolloverScanner.scanPatternMatches(chartVals);

	for(PatternMatchList::const_iterator rolloverMatchIter = rolloverMatches->begin();
			rolloverMatchIter!=rolloverMatches->end();rolloverMatchIter++)
	{
		CupScannerConfiguratorPtr cupScannerConfig(new CupScannerConfigurator());

		// The up-trend on the cup pattern must go beyond the highest high of
		// the rollover pattern.
		double rolloverHighestHigh = (*rolloverMatchIter)->highestHigh();
		cupScannerConfig->addUpTrendValidator(lastHighAboveFixedValue(rolloverHighestHigh));
		CupScanner cupScanner(cupScannerConfig);

		PeriodValSegmentPtr valsForCupScan = (*rolloverMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr cupMatches = cupScanner.scanPatternMatches(valsForCupScan);

		for(PatternMatchList::const_iterator cupMatchIter = cupMatches->begin();
				cupMatchIter!=cupMatches->end();cupMatchIter++)
		{
			PatternMatchPtr overallMatch = ((*rolloverMatchIter)->appendMatch(**cupMatchIter));
			if(overallDepthValidator->validPattern(*overallMatch))
			{
				pullbackMatches->push_back(overallMatch);
			}
		} // for each cup match
	} // for each roll-over match

	// If price doesn't first roll-over, it can do a straight pull-back into a cup-shaped pull-back.
	CupScannerConfiguratorPtr cupScannerConfig(new CupScannerConfigurator());
	cupScannerConfig->addOverallValidator(lastHighAboveFirstHigh());
	cupScannerConfig->addOverallValidator(overallDepthValidator);
	CupScanner cupShapedPullbackScanner(cupScannerConfig);
	PatternMatchListPtr cupMatches = cupShapedPullbackScanner.scanPatternMatches(chartVals);
	pullbackMatches->insert(pullbackMatches->end(),cupMatches->begin(),cupMatches->end());

	VScanner vShapedPullbackScanner;
	vShapedPullbackScanner.addOverallValidator(lastHighAboveFirstHigh());
	vShapedPullbackScanner.addOverallValidator(overallDepthValidator);
	PatternMatchListPtr vMatches = vShapedPullbackScanner.scanPatternMatches(chartVals);
	pullbackMatches->insert(pullbackMatches->end(),vMatches->begin(),vMatches->end());

	return filterUniqueMatches(pullbackMatches);

}

AscendingBasePullbackScanner::~AscendingBasePullbackScanner() {
}

