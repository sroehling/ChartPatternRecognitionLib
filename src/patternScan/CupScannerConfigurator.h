/*
 * CupScannerConfigurator.h
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#ifndef CUPSCANNERCONFIGURATOR_H_
#define CUPSCANNERCONFIGURATOR_H_

#include "DoubleRange.h"
#include "PatternMatch.h"
#include "PatternMatchValidator.h"
#include <boost/shared_ptr.hpp>


class CupScannerConfigurator {
private:

public:
	CupScannerConfigurator() {}

	virtual PatternMatchValidatorPtr upTrendValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &flatTrend) const;

	virtual PatternMatchValidatorPtr overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &flatTrend, const PatternMatchPtr &upTrend) const;

	virtual ~CupScannerConfigurator() {}
};

typedef boost::shared_ptr<CupScannerConfigurator> CupScannerConfiguratorPtr;

#endif /* CUPSCANNERCONFIGURATOR_H_ */
