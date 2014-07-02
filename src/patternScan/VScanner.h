/*
 * VScanner.h
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#ifndef VSCANNER_H_
#define VSCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScanner.h"
#include "PatternMatchValidator.h"

// Scan for V shaped patterns. This class is a building block
// for double bottoms or other patterns with a V shape.
class VScanner {
private:
	// Minimum amount the RHS of the V (uptrend) can be below the LHS (downtrend).
	double minRHSBelowLHSofVPerc_;

	PatternMatchValidatorPtr uptrendPercOfDowntrendValidator(const PatternMatchPtr &downtrendMatch) const;

public:
	VScanner(double minRHSBelowLHSofVPerc);
	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
	virtual ~VScanner();
};

#endif /* VSCANNER_H_ */
