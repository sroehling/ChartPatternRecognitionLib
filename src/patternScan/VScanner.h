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
#include "CompositePatternMatchValidatorFactory.h"

// Scan for V shaped patterns. This class is a building block
// for double bottoms or other patterns with a V shape.
class VScanner {
private:
	PatternMatchValidatorList customOverallValidators_;

	CompositePatternMatchValidatorFactory upTrendValidatorFactory_;

protected:
	virtual PatternMatchValidatorPtr overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &upTrend) const;

public:
	VScanner();

	// Add a custom/user-defined "static" up-trend validators; i.e., one which doesn't
	// depend on the downTrend or flatTrend, but is appended to the list of
	// validators AND'ed to any other up-trend validators.
	void addUpTrendValidator(const PatternMatchValidatorPtr &upTrendValidator);
	void addOverallValidator(const PatternMatchValidatorPtr &overallValidator);

	CompositePatternMatchValidatorFactory &upTrendValidatorFactory() { return upTrendValidatorFactory_; }


	virtual PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;
	virtual ~VScanner();
};

#endif /* VSCANNER_H_ */
