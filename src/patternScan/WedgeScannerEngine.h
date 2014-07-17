/*
 * WedgeScannerEngine.h
 *
 *  Created on: Jul 16, 2014
 *      Author: sroehling
 */

#ifndef WEDGESCANNERENGINE_H_
#define WEDGESCANNERENGINE_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"

// WedgeScannerEngine is a scanner for a category of patterns where 2 trend lines come together in an
// apex. This can include descending triangle (downward upper trend line, flat lower trend line),
// ascending triangle (flat upper trend line, upward sloping lower trend line), or
// triangle (downward sloping upper trend line, upward sloping lower trend line).
class WedgeScannerEngine {
public:
	WedgeScannerEngine();

	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~WedgeScannerEngine();
};

#endif /* WEDGESCANNERENGINE_H_ */
