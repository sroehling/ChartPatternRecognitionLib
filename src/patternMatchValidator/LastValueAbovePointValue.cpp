/*
 * LastValueCloseAbovePointValue.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#include "LastValueAbovePointValue.h"

LastValueAbovePointValue::LastValueAbovePointValue(const PeriodValueRefPtr &perValRef, double thresholdPointValue)
: perValRef_(perValRef),
thresholdPointValue_(thresholdPointValue)
{
	assert(thresholdPointValue_ >= 0.0);

}

bool LastValueAbovePointValue::validPattern(const PatternMatch &candidateMatch)
{
	return (perValRef_->referencedVal(candidateMatch.lastValue()) >= thresholdPointValue_)?true:false;
}


LastValueAbovePointValue::~LastValueAbovePointValue() {
}

