/*
 * WedgeScannerEngine.cpp
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#include <WedgeScannerEngine.h>
#include "VScanner.h"
#include "InvertedVScanner.h"
#include "PeriodValSegment.h"
#include "MultiPatternScanner.h"

WedgeScannerEngine::WedgeScannerEngine() {
}

PatternMatchListPtr WedgeScannerEngine::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	PatternMatchListPtr wedgeMatches(new PatternMatchList());

	// Unlike the PatternScannerEngine, this scanner will return pattern matches starting
	// anywhere from the beginning of chartVals to the end.

	// Step 1: Scan chartVals for a list of pivot lows and pivot highs.
	// These pivots serve as anchor points to draw potential
	// trend-lines forming the patterns.

	return wedgeMatches; // stubbed-out.

}


WedgeScannerEngine::~WedgeScannerEngine() {
}

