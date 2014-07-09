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

AscendingBasePullbackScanner::AscendingBasePullbackScanner() {
	// TODO Auto-generated constructor stub

}

PatternMatchListPtr AscendingBasePullbackScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr pullbackMatches(new PatternMatchList());

	// An optional short flat area leading into the pullback.
	PatternScannerPtr flatScanner(new TrendLineScanner(TrendLineScanner::FLAT_SLOPE_RANGE));
	PatternMatchListPtr flatMatches = flatScanner->scanPatternMatches(chartVals);

	for(PatternMatchList::const_iterator flatMatchIter = flatMatches->begin();
			flatMatchIter!=flatMatches->end();flatMatchIter++)
	{
		CupScannerConfiguratorPtr cupScannerConfig(new CupScannerConfigurator());
		CupScanner cupScanner(cupScannerConfig);
	}

	return pullbackMatches;

}

AscendingBasePullbackScanner::~AscendingBasePullbackScanner() {
	// TODO Auto-generated destructor stub
}

