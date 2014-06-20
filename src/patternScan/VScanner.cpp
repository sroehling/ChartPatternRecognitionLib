/*
 * VScanner.cpp
 *
 *  Created on: Jun 20, 2014
 *      Author: sroehling
 */

#include "VScanner.h"

VScanner::VScanner() {

}

PatternMatchListPtr VScanner::scanPatternMatches(const PeriodValSegmentPtr &chartVals) const
{
	return PatternMatchListPtr(new PatternMatchList()); // dummied-up
}


VScanner::~VScanner() {
}

