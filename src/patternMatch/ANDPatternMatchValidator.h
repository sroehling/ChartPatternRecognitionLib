/*
 * ANDPatternMatchValidator.h
 *
 *  Created on: Jul 8, 2014
 *      Author: sroehling
 */

#ifndef ANDPATTERNMATCHVALIDATOR_H_
#define ANDPATTERNMATCHVALIDATOR_H_

#include <PatternMatchValidator.h>

class ANDPatternMatchValidator: public PatternMatchValidator {
private:
	PatternMatchValidatorList andList_;
public:
	ANDPatternMatchValidator(const PatternMatchValidatorList &andList);
	virtual bool validPattern(const PatternMatch &candidateMatch);
	virtual ~ANDPatternMatchValidator();
};

#endif /* ANDPATTERNMATCHVALIDATOR_H_ */
