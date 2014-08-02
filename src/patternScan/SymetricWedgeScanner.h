/*
 * SymetricWedgeScanner.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef SYMETRICWEDGESCANNER_H_
#define SYMETRICWEDGESCANNER_H_

#include <WedgeScanner.h>

class SymetricWedgeScanner: public WedgeScanner {
protected:
	virtual bool validTrendLines(const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine) const;

	// Test if a completed pattern match is found at the location pointed to by
	// currPerValIter.
	virtual PatternMatchPtr findPatternMatch(const PeriodValSegmentPtr &chartVals,
			const ChartSegmentPtr &upperTrendLine,
			const ChartSegmentPtr &lowerTrendLine,
			const PeriodValCltn::iterator &currPerValIter) const;
public:
};

#endif /* SYMETRICWEDGESCANNER_H_ */
