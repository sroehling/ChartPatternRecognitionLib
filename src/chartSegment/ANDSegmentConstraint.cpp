/*
 * ANDSegmentContraint.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#include <ANDSegmentConstraint.h>



ANDSegmentConstraint::ANDSegmentConstraint(const SegmentConstraintList &constraints)
: constraints_(constraints)
{
}

bool ANDSegmentConstraint::validSegment(const ChartSegment &segment)
{
	for(SegmentConstraintList::const_iterator conIter = constraints_.begin();
				conIter != constraints_.end(); conIter++)
	{
		if(!((*conIter)->validSegment(segment)))
		{
			return false;
		}
	}
	return true;
}

ANDSegmentConstraint::~ANDSegmentConstraint() {
}

