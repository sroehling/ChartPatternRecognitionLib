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

class PatternMatchValidator;
typedef boost::shared_ptr<PatternMatchValidator> PatternMatchValidatorPtr;
typedef std::list<PatternMatchValidatorPtr> PatternMatchValidatorList;


// The PatternMatchValidator class is a base class for classes
// which validate a pattern as a whole, given a candidate match
// (e.g., based upon length, % change since start, etc).
class PatternMatchValidator {
public:
	PatternMatchValidator();

	virtual bool validPattern(const PatternMatch &candidateMatch) = 0;

	static PatternMatchListPtr filterMatches(const PatternMatchValidatorPtr &validator,
			const PatternMatchListPtr &unfilteredMatches);

	virtual ~PatternMatchValidator();
};


#endif /* PATTERNMATCHVALIDATOR_H_ */
