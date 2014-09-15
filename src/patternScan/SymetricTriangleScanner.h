/*
 * SymetricTriangleScanner.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef SYMETRICTRIANGLESCANNER_H_
#define SYMETRICTRIANGLESCANNER_H_

#include <TriangleScanner.h>

class SymetricTriangleScanner: public TriangleScanner {
protected:

	// Test if a completed pattern match is found at the location pointed to by
	// currPerValIter.
    virtual PatternMatchPtr findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    SymetricTriangleScanner();
};

#endif /* SYMETRICTRIANGLESCANNER_H_ */
