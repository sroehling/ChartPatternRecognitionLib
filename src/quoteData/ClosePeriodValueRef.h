/*
 * ClosePeriodValueRef.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#ifndef CLOSEPERIODVALUEREF_H_
#define CLOSEPERIODVALUEREF_H_

#include <PeriodValueRef.h>

class ClosePeriodValueRef: public PeriodValueRef {
public:
	ClosePeriodValueRef();

	virtual double referencedVal(const PeriodVal &periodVal) const;

	virtual ~ClosePeriodValueRef();
};

#endif /* CLOSEPERIODVALUEREF_H_ */
