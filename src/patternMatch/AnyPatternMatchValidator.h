/*
 * AnyPatternMatchValidator.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sroehling
 */

#ifndef ANYPATTERNMATCHVALIDATOR_H_
#define ANYPATTERNMATCHVALIDATOR_H_

#include <PatternMatchValidator.h>

// PatternMatchValidator for testing - always return true
// for validPattern.
class AnyPatternMatchValidator: public PatternMatchValidator {
public:
	AnyPatternMatchValidator();
	virtual bool validPattern(const PatternMatch &candidateMatch);
	virtual ~AnyPatternMatchValidator();
};

#endif /* ANYPATTERNMATCHVALIDATOR_H_ */
