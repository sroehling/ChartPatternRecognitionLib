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
	PatternMatchValidatorList customUpTrendValidators_;
	PatternMatchValidatorList customOverallValidators_;
public:
	CupScannerConfigurator() {}

	// Add a custom/user-defined "static" up-trend validators; i.e., one which doesn't
	// depend on the downTrend or flatTrend, but is appended to the list of
	// validators AND'ed to any other up-trend validators.
	void addUpTrendValidator(const PatternMatchValidatorPtr &upTrendValidator);

	virtual PatternMatchValidatorPtr upTrendValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &flatTrend) const;

	// Add a custom/user-defined overal validator (see description for
	// addUpTrendValidator.
	void addOverallValidator(const PatternMatchValidatorPtr &overallValidator);

	virtual PatternMatchValidatorPtr overallValidator(const PatternMatchPtr &downTrend,
			const PatternMatchPtr &flatTrend, const PatternMatchPtr &upTrend) const;

	virtual ~CupScannerConfigurator() {}
};

typedef boost::shared_ptr<CupScannerConfigurator> CupScannerConfiguratorPtr;

#endif /* CUPSCANNERCONFIGURATOR_H_ */
