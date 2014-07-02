/*
 * DoubleBottomScanner.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: sroehling
 */

#include <DoubleBottomScanner.h>
#include "VScanner.h"
#include "PatternMatch.h"

DoubleBottomScanner::DoubleBottomScanner() {
}

PatternMatchListPtr DoubleBottomScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{

	// The 1st up-trend of the double-bottom must no more than 40% below the depth of the
	// 1st down-trend.
	double minPercentOfDowntrend = 40.0;
	VScanner leftVScanner(minPercentOfDowntrend);
	PatternMatchListPtr leftVMatches = leftVScanner.scanPatternMatches(chartVals);

	PatternMatchListPtr dblBottomMatches(new PatternMatchList());

	for(PatternMatchList::const_iterator leftMatchIter = leftVMatches->begin();
			leftMatchIter!=leftVMatches->end();leftMatchIter++)
	{
		PeriodValSegmentPtr valsForRightVScan = (*leftMatchIter)->trailingValsWithLastVal();

		// The RHS of the RHS V must recover 100% of of the LHS of this V. In other words,
		// for the pattern match to be complete (confirmed), the right-most up-trend of the
		// double-bottom must exceed the start of the 2nd down-trend of the double-bottom.
		double rhsVMinRecoverLHSDepth = 0.0; // RHS of RHS V must recover 100% of the depth of the LHS of V
		VScanner rightVScanner(rhsVMinRecoverLHSDepth);

		PatternMatchListPtr rightVMatches = rightVScanner.scanPatternMatches(valsForRightVScan);

		// Put the left V and Right V pattern matches together, forming the overall double-bottom pattern.
		PatternMatchListPtr overallMatches = (*leftMatchIter)->appendMatchList(*rightVMatches);

		// TODO - Probably some overall validation is needed here; e.g., the overall
		// depth of the double bottom should not exceed 30%.
		dblBottomMatches->insert(dblBottomMatches->end(),overallMatches->begin(),overallMatches->end());
	} // for each "left V" match.

	return dblBottomMatches;
}

DoubleBottomScanner::~DoubleBottomScanner() {
}

