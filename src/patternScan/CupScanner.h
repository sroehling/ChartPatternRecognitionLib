/*
 * CupScanner.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#ifndef CUPSCANNER_H_
#define CUPSCANNER_H_

#include "PatternMatch.h"
#include "PeriodValSegment.h"
#include "PatternScannerEngine.h"
#include "CupScannerConfigurator.h"


class CupScanner {
private:
	CupScannerConfiguratorPtr configurator_;
public:
	CupScanner(const CupScannerConfiguratorPtr &configurator);

	PatternMatchListPtr scanPatternMatches(const PeriodValSegmentPtr &chartVals) const;

	virtual ~CupScanner() {}
};

#endif /* CUPSCANNER_H_ */
