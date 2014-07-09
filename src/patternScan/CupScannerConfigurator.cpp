/*
 * CupScannerConfigurator.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#include <CupScannerConfigurator.h>
#include "AnyPatternMatchValidator.h"
#include "ORPatternMatchValidator.h"
#include "ANDPatternMatchValidator.h"
#include "EndWithinPercentOfStart.h"

void CupScannerConfigurator::addUpTrendValidator(const PatternMatchValidatorPtr &upTrendValidator)
{
	customUpTrendValidators_.push_back(upTrendValidator);
}

PatternMatchValidatorPtr CupScannerConfigurator::upTrendValidator(const PatternMatchPtr &downTrend,
		const PatternMatchPtr &flatTrend) const
{
	return PatternMatchValidatorPtr(new ANDPatternMatchValidator(customUpTrendValidators_));
}

void CupScannerConfigurator::addOverallValidator(const PatternMatchValidatorPtr &overallValidator)
{
	customOverallValidators_.push_back(overallValidator);
}

PatternMatchValidatorPtr CupScannerConfigurator::overallValidator(const PatternMatchPtr &downTrend,
		const PatternMatchPtr &flatTrend, const PatternMatchPtr &upTrend) const
{
	return PatternMatchValidatorPtr(new ANDPatternMatchValidator(customOverallValidators_));
}


