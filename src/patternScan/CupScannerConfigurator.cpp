/*
 * CupScannerConfigurator.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: sroehling
 */

#include <CupScannerConfigurator.h>
#include "AnyPatternMatchValidator.h"
#include "ORPatternMatchValidator.h"
#include "EndWithinPercentOfStart.h"

PatternMatchValidatorPtr CupScannerConfigurator::upTrendValidator(const PatternMatchPtr &downTrend,
		const PatternMatchPtr &flatTrend) const
{
	return PatternMatchValidatorPtr(new AnyPatternMatchValidator());
}

PatternMatchValidatorPtr CupScannerConfigurator::overallValidator(const PatternMatchPtr &downTrend,
		const PatternMatchPtr &flatTrend, const PatternMatchPtr &upTrend) const
{

	PatternMatchValidatorList finalValidators;
	finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(8.0)));
	finalValidators.push_back(PatternMatchValidatorPtr(new EndWithinPercentOfStart(-3.0)));
	PatternMatchValidatorPtr overallValidator(new ORPatternMatchValidator(finalValidators));

	return overallValidator;
}


