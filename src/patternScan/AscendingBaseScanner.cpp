/*
 * AscendingBaseScanner.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#include <AscendingBaseScanner.h>
#include "AscendingBasePullbackScanner.h"
#include "ScannerHelper.h"
#include "CompositePatternMatchValidatorFactory.h"
#include "HigherLowPatternMatchValidatorFactory.h"

using namespace scannerHelper;

AscendingBaseScanner::AscendingBaseScanner() {
}


PatternMatchListPtr AscendingBaseScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	DoubleRange ascendingBaseDepthRange(10.0,20.0);
	CompositePatternMatchValidatorFactory pullbackValidatorFactory;
	pullbackValidatorFactory.addFactory(
			PatternMatchValidatorFactoryPtr(new HigherLowPatternMatchValidatorFactory()));

	PatternMatchListPtr ascendingBaseMatches(new PatternMatchList());

	AscendingBasePullbackScanner pullbackScanner(ascendingBaseDepthRange);

	PatternMatchListPtr pullback1Matches = pullbackScanner.scanPatternMatches(chartVals);

	for(PatternMatchList::const_iterator pullback1MatchIter = pullback1Matches->begin();
			pullback1MatchIter!=pullback1Matches->end();pullback1MatchIter++)
	{
		PeriodValSegmentPtr valsFor2ndPullbackScan = (*pullback1MatchIter)->trailingValsWithLastVal();
		PatternMatchValidatorPtr pullback2Validator = pullbackValidatorFactory.createValidator1(*pullback1MatchIter);
		PatternMatchListPtr pullback2Matches = PatternMatchValidator::filterMatches(pullback2Validator,
				pullbackScanner.scanPatternMatches(valsFor2ndPullbackScan));

		for(PatternMatchList::const_iterator pullback2MatchIter = pullback2Matches->begin();
				pullback2MatchIter!=pullback2Matches->end();pullback2MatchIter++)
		{
			PeriodValSegmentPtr valsFor3rdPullbackScan = (*pullback2MatchIter)->trailingValsWithLastVal();
			PatternMatchValidatorPtr pullback3Validator = pullbackValidatorFactory.createValidator1(*pullback2MatchIter);
			PatternMatchListPtr pullback3Matches = PatternMatchValidator::filterMatches(pullback3Validator,
					pullbackScanner.scanPatternMatches(valsFor3rdPullbackScan));

			for(PatternMatchList::const_iterator pullback3MatchIter = pullback3Matches->begin();
					pullback3MatchIter!=pullback3Matches->end();pullback3MatchIter++)
			{

				PatternMatchPtr overallMatch = (*pullback1MatchIter)->appendMatch(**pullback2MatchIter)
								->appendMatch(**pullback3MatchIter);
				// TODO Validate the overall match for length.

				ascendingBaseMatches->push_back(overallMatch);

			} // For each 3rd pull-back
		} // for each match of the 2nd pull back
	} // for each match in the first pull back

	return filterUniqueMatches(ascendingBaseMatches);
}

