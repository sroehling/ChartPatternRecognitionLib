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
	PatternMatchValidatorList customUpTrendValidators_;
protected:
	virtual PatternMatchValidatorPtr upTrendValidator(const PatternMatchPtr &downTrend) const;
	virtual PatternMatchValidatorPtr overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &upTrend) const;
private:
	// Minimum amount the RHS of the V (uptrend) can be below the LHS (downtrend).
	double minRHSBelowLHSofVPerc_;

	// Add a custom/user-defined "static" up-trend validators; i.e., one which doesn't
	// depend on the downTrend or flatTrend, but is appended to the list of
	// validators AND'ed to any other up-trend validators.
	void addUpTrendValidator(const PatternMatchValidatorPtr &upTrendValidator);


	PatternMatchValidatorPtr uptrendPercOfDowntrendValidator(const PatternMatchPtr &downtrendMatch) const;

public:
	VScanner(double minRHSBelowLHSofVPerc);
	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
	virtual ~VScanner();
};

#endif /* VSCANNER_H_ */
