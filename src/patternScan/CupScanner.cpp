/*
 * CupScanner.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#include <CupScanner.h>
#include "SegmentConstraint.h"
#include "SlopeWithinRangeConstraint.h"
#include "SegmentValsCloseToLinearEq.h"
#include "ANDSegmentConstraint.h"
#include "AnySegmentListValidConstraint.h"
#include "AnyPatternMatchValidator.h"
#include "PeriodValSegment.h"
#include "EndWithinPercentOfStart.h"
#include "ORPatternMatchValidator.h"

CupScanner::CupScanner() {

}

PatternScannerPtr trendLineScanner(double minSlope, double maxSlope)
{
	SegmentConstraintPtr valsCloseToEquation(new SegmentValsCloseToLinearEq(4.5));
	SegmentConstraintPtr trendSlope(new SlopeWithinRangeConstraint(minSlope,maxSlope));
	SegmentConstraintList trendConstraints;
	trendConstraints.push_back(valsCloseToEquation);
	trendConstraints.push_back(trendSlope);
	SegmentConstraintPtr trendSegConstraints(new ANDSegmentConstraint(trendConstraints));
	SegmentListConstraintPtr trendSegListConstraint(new AnySegmentListValidConstraint());
	PatternMatchValidatorPtr trendPatternMatchValidator(new AnyPatternMatchValidator());

	PatternScannerPtr trendScanner(new PatternScanner(trendSegConstraints,trendSegListConstraint,trendPatternMatchValidator));

	return trendScanner;
}

PatternMatchListPtr CupScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	PatternScannerPtr downtrendScanner = trendLineScanner(-1.2,-0.5);
	PatternScannerPtr flatScanner = trendLineScanner(-0.5,0.5);
	PatternScannerPtr uptrendScanner = trendLineScanner(0.5,1.2);

	PatternMatchListPtr cupMatches(new PatternMatchList());

	PatternMatchListPtr downtrendMatches = downtrendScanner->scanPatternMatches(chartVals);
//	std::cerr << "Cup Scanner: downtrend matches = " << downtrendMatches->size() << std::endl;
	unsigned int downtrendNum = 0;
	for(PatternMatchList::const_iterator dtMatchIter = downtrendMatches->begin();
				dtMatchIter!=downtrendMatches->end();dtMatchIter++)
	{
//		std::cerr << "Downtrend match: " << (**dtMatchIter) <<  std::endl;
		PeriodValSegmentPtr valsForFlatScan = (*dtMatchIter)->trailingValsWithLastVal();
		PatternMatchListPtr flatMatches = flatScanner->scanPatternMatches(valsForFlatScan);
		PatternMatchListPtr downFlatMatches = (*dtMatchIter)->appendMatchList(*flatMatches);
//		std::cerr << "Cup Scanner: down & flat trend matches = " << downFlatMatches->size() << std::endl;
		for(PatternMatchList::const_iterator downFlatMatchIter = downFlatMatches->begin();
				downFlatMatchIter!=downFlatMatches->end();downFlatMatchIter++)
		{
//			std::cerr << "Down and flat trend match: " << (**downFlatMatchIter) <<  std::endl;
			PeriodValSegmentPtr valsForUptrendScan = (*downFlatMatchIter)->trailingValsWithLastVal();
			PatternMatchListPtr uptrendMatches = uptrendScanner->scanPatternMatches(valsForUptrendScan);

			PatternMatchListPtr overallMatches = (*downFlatMatchIter)->appendMatchList(*uptrendMatches);

			// Perform a final validation on the pattern as a whole.
			PatternMatchValidatorList finalValidators;
			finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(5.0)));
			finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(-5.0)));
			ORPatternMatchValidator plusMinusWithinStart(finalValidators);
			for(PatternMatchList::const_iterator overallIter = overallMatches->begin();
						overallIter != overallMatches->end(); overallIter++)
			{
				if(plusMinusWithinStart.validPattern(**overallIter))
				{
					cupMatches->push_back(*overallIter);
				}

			}

		} // For each combined down trend and flat area match

		downtrendNum++;
	} // For each down trend match

	return cupMatches;
}

CupScanner::~CupScanner() {
}

