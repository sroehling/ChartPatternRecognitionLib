/*
 * PatternMatchValidator.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef PATTERNMATCHVALIDATOR_H_
#define PATTERNMATCHVALIDATOR_H_

#include <boost/shared_ptr.hpp>
#include "PatternMatch.h"

// The PatternMatchValidator class is a base class for classes
// which validate a pattern as a whole, given a candidate match
// (e.g., based upon length, % change since start, etc).
class PatternMatchValidator {
public:
	PatternMatchValidator();

	virtual bool validPattern(const PatternMatch &candidateMatch) = 0;

	virtual ~PatternMatchValidator();
};

typedef boost::shared_ptr<PatternMatchValidator> PatternMatchValidatorPtr;

#endif /* PATTERNMATCHVALIDATOR_H_ */
