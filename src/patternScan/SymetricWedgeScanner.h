/*
 * SymetricWedgeScanner.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef SYMETRICWEDGESCANNER_H_
#define SYMETRICWEDGESCANNER_H_

#include <WedgeScanner.h>

class SymetricWedgeScanner: public WedgeScanner {
protected:

	// Test if a completed pattern match is found at the location pointed to by
	// currPerValIter.
    virtual PatternMatchPtr findPatternMatch(const WedgeMatchValidationInfo &wedgeMatchValidationInfo) const;
public:
    SymetricWedgeScanner();
};

#endif /* SYMETRICWEDGESCANNER_H_ */
