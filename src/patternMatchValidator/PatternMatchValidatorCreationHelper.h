/*
 * PatternMatchValidatorCreationHelper.h
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVALIDATORCREATIONHELPER_H_
#define PATTERNMATCHVALIDATORCREATIONHELPER_H_

#include "PatternMatch.h"
#include "PatternMatchValueRef.h"
#include "ValueComparisonMatchValidator.h"
#include "DoubleRange.h"
#include "PeriodValueRef.h"

namespace patternMatchValidatorCreationHelper
{
	PatternMatchValidatorPtr depthWithinRangeValidator(const DoubleRange &minMaxDepthPerc);
    PatternMatchValidatorPtr depthSinceStartWithinRangeValidator(const DoubleRange &minMaxDepthPerc);

	// In comparison with another pattern 'compareWith', validate that the given
	// pattern has a lower low than compareWith.
	PatternMatchValidatorPtr lowerLowValidator(const PatternMatchPtr &compareWith);

    PatternMatchValidatorPtr lowAbovePercDepthOfOtherPattern(const PatternMatchPtr &otherPattern, double percDepthOther);

	PatternMatchValidatorPtr lastHighAboveFixedValue(double thresholdValue);
	PatternMatchValidatorPtr lastHighAboveFirstHigh();

    PatternMatchValidatorPtr highestHighBelowLastHigh();

    PatternMatchValidatorPtr highestCloseBelowFirstHigh();

}


#endif /* PATTERNMATCHVALIDATORCREATIONHELPER_H_ */
