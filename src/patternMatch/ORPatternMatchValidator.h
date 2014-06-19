/*
 * ORPatternMatchValidator.h
 *
 *  Created on: Jun 19, 2014
 *      Author: sroehling
 */

#ifndef ORPATTERNMATCHVALIDATOR_H_
#define ORPATTERNMATCHVALIDATOR_H_

#include <PatternMatchValidator.h>

// PatternMatchValidator which performs a boolean OR for
// the validPattern() results for the validators in orList.
class ORPatternMatchValidator: public PatternMatchValidator {
private:
	PatternMatchValidatorList orList_;
public:
	ORPatternMatchValidator(const PatternMatchValidatorList &orList);

	virtual bool validPattern(const PatternMatch &candidateMatch);

	virtual ~ORPatternMatchValidator();
};

#endif /* ORPATTERNMATCHVALIDATOR_H_ */
