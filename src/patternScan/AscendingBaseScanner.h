/*
 * AscendingBaseScanner.h
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#ifndef ASCENDINGBASESCANNER_H_
#define ASCENDINGBASESCANNER_H_

#include <PatternScanner.h>

class AscendingBaseScanner: public PatternScanner {
public:
	AscendingBaseScanner();

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~AscendingBaseScanner() {}
};

#endif /* ASCENDINGBASESCANNER_H_ */
