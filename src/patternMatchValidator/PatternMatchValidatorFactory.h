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

	virtual PatternMatchValidatorPtr createValidator(const PatternMatchVector &previousMatches) const = 0;

	// Convenience/helper methods for creating validators with a certain number of previous
	// matches.
	PatternMatchValidatorPtr createValidator1(const PatternMatchPtr &otherMatch) const;
	PatternMatchValidatorPtr createValidator2(const PatternMatchPtr &firstMatch,
			const PatternMatchPtr &secondMatch) const;
	PatternMatchValidatorPtr createValidator3(const PatternMatchPtr &firstMatch,
			const PatternMatchPtr &secondMatch,const PatternMatchPtr &thirdMatch) const;

	PatternMatchValidatorPtr createValidator(const PatternMatchPtr &firstMatch,
			const PatternMatchPtr &secondMatch,const PatternMatchPtr &thirdMatch) const;

	virtual ~PatternMatchValidatorFactory();
};

typedef boost::shared_ptr<PatternMatchValidatorFactory> PatternMatchValidatorFactoryPtr;
typedef std::list<PatternMatchValidatorFactoryPtr> PatternMatchValidatorFactoryList;

#endif /* PATTERNMATCHVALIDATORFACTORY_H_ */
