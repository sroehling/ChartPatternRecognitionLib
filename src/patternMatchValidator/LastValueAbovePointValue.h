/*
 * LastValueCloseAbovePointValue.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#ifndef LASTVALUEABOVEPOINTVALUE_H_
#define LASTVALUEABOVEPOINTVALUE_H_

#include "PatternMatchValidator.h"
#include "PeriodValueRef.h"

class LastValueAbovePointValue: public PatternMatchValidator {
private:
	// Parameterize which value from the PeriodVal is used is used for comparison
	// (e.g., high, low close).
	PeriodValueRefPtr perValRef_;

	double thresholdPointValue_;
public:
	LastValueAbovePointValue(const PeriodValueRefPtr &perValRef, double thresholdPointValue);

	bool validPattern(const PatternMatch &candidateMatch);

	virtual ~LastValueAbovePointValue();
};

#endif /* LASTVALUEABOVEPOINTVALUE_H_ */
