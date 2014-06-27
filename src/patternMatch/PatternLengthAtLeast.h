/*
 * PatternLengthAtLeast.h
 *
 *  Created on: Jun 26, 2014
 *      Author: sroehling
 */

#ifndef PATTERNLENGTHATLEAST_H_
#define PATTERNLENGTHATLEAST_H_

#include <PatternMatchValidator.h>

class PatternLengthAtLeast: public PatternMatchValidator {
private:
	unsigned int minPeriods_;
public:
	PatternLengthAtLeast(unsigned int minPeriods);
	virtual bool validPattern(const PatternMatch &candidateMatch);
	virtual ~PatternLengthAtLeast();
};

#endif /* PATTERNLENGTHATLEAST_H_ */
