/*
 * PatternMatchValidatorCreationHelper.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#include <PatternMatchValidatorCreationHelper.h>
#include "ANDPatternMatchValidator.h"

namespace PatternMatchValidatorCreationHelper
{


PatternMatchValidatorPtr minDepthPercentValidator(double minDepthPerc)
{
	assert(minDepthPerc >= 0.0);

	ValueComparatorPtr greaterEqualCompare(new GreaterThanEqualValueComparator());
	PatternMatchValueRefPtr depthVal(new DepthPercentPatternMatchValueRef());
	PatternMatchValueRefPtr minDepthVal(new FixedPatternMatchValueRef(minDepthPerc));
	PatternMatchValidatorPtr minDepthValidator(
			new ValueComparisonMatchValidator(depthVal,minDepthVal,greaterEqualCompare));
	return minDepthValidator;
}

PatternMatchValidatorPtr maxDepthPercentValidator(double maxDepthPerc)
{
	assert(maxDepthPerc >= 0.0);

	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr maxDepthVal(new FixedPatternMatchValueRef(maxDepthPerc));
	PatternMatchValueRefPtr depthVal(new DepthPercentPatternMatchValueRef());
	PatternMatchValidatorPtr maxDepthValidator(
			new ValueComparisonMatchValidator(depthVal,maxDepthVal,lessEqualCompare));
	return maxDepthValidator;
}


PatternMatchValidatorPtr depthWithinRangeValidator(const DoubleRange &minMaxDepthPerc)
{
	assert(minMaxDepthPerc.positiveVals());

	PatternMatchValidatorPtr minDepth = minDepthPercentValidator(minMaxDepthPerc.minVal());
	PatternMatchValidatorPtr maxDepth = maxDepthPercentValidator(minMaxDepthPerc.maxVal());

	PatternMatchValidatorList andList;
	andList.push_back(minDepth);
	andList.push_back(maxDepth);

	PatternMatchValidatorPtr depthInRange(new ANDPatternMatchValidator(andList));
	return depthInRange;
}

PatternMatchValidatorPtr lowerLowValidator(const PatternMatchPtr &compareWith) const
{
	ValueComparatorPtr lessEqualCompare(new LessThanEqualValueComparator());
	PatternMatchValueRefPtr compareWithLow(new FixedPatternMatchValueRef(compareWith->lowestLow()));
	PatternMatchValueRefPtr validationLowRef(new LowestLowPatternMatchValueRef());
	PatternMatchValidatorPtr lowerLowValidator(
			new ValueComparisonMatchValidator(validationLowRef,compareWithLow,lessEqualCompare));
	return lowerLowValidator;
}


}
