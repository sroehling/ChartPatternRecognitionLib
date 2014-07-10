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


namespace patternMatchValidatorCreationHelper
{
	PatternMatchValidatorPtr minDepthPercentValidator(double minDepthPerc);
	PatternMatchValidatorPtr maxDepthPercentValidator(double maxDepthPerc);
	PatternMatchValidatorPtr depthWithinRangeValidator(const DoubleRange &minMaxDepthPerc);

	// In comparison with another pattern 'compareWith', validate that the given
	// pattern has a lower low than compareWith.
	PatternMatchValidatorPtr lowerLowValidator(const PatternMatchPtr &compareWith);

	PatternMatchValidatorPtr lastHighAboveFixedValue(double thresholdValue);
	PatternMatchValidatorPtr lastHighAboveFirstHigh();

}


#endif /* PATTERNMATCHVALIDATORCREATIONHELPER_H_ */
