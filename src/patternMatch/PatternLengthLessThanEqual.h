/*
 * PatternLengthLessThanEqual.h
 *
 *  Created on: Jun 27, 2014
 *      Author: sroehling
 */

#ifndef PATTERNLENGTHLESSTHANEQUAL_H_
#define PATTERNLENGTHLESSTHANEQUAL_H_

#include <PatternMatchValidator.h>

class PatternLengthLessThanEqual: public PatternMatchValidator {
private:
	unsigned int maxPeriods_;
public:
	PatternLengthLessThanEqual(unsigned int maxPeriods);
	virtual bool validPattern(const PatternMatch &candidateMatch);
	virtual ~PatternLengthLessThanEqual();
};

#endif /* PATTERNLENGTHLESSTHANEQUAL_H_ */
