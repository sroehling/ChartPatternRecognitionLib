/*
 * AscendingBasePullbackScanner.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#include <AscendingBasePullbackScanner.h>
#include "CupScanner.h"
#include "CupScannerConfigurator.h"
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
		}


	}

	return filterUniqueMatches(pullbackMatches);

}

AscendingBasePullbackScanner::~AscendingBasePullbackScanner() {
}

