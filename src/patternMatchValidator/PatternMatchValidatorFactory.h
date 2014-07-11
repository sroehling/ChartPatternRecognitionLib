/*
 * PatternMatchValidatorFactory.h
 *
 *  Created on: Jul 11, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVALIDATORFACTORY_H_
#define PATTERNMATCHVALIDATORFACTORY_H_

#include "PatternMatch.h"
#include "PatternMatchValidator.h"

#include <boost/shared_ptr.hpp>

// The various pattern scanner classes often need (derived) PatternMatchValidator's
// which are constructed & configured based upon a previous pattern. For example,
// DoubleBottomScanner configures the VScanner on the LHS so it's uptrend must
// recover a certain percentage of the initial downtrend. Rather than hard-code
// the various PatternMatchValidators in the scanner classes, the PatternMatchValidatorFactory
// allows the sub-pattern scanners such as VScanner to be configured for the type
// of validation behavior they need.
class PatternMatchValidatorFactory {
public:
	PatternMatchValidatorFactory();

	virtual PatternMatchValidatorPtr createValidator(const PatternMatchPtr &otherMatch) const = 0;

	virtual ~PatternMatchValidatorFactory();
};

typedef boost::shared_ptr<PatternMatchValidatorFactory> PatternMatchValidatorFactoryPtr;
typedef std::list<PatternMatchValidatorFactoryPtr> PatternMatchValidatorFactoryList;

#endif /* PATTERNMATCHVALIDATORFACTORY_H_ */
