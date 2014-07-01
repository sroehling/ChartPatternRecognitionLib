/*
 * PeriodValueRef.h
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#ifndef PERIODVALUEREF_H_
#define PERIODVALUEREF_H_

#include "PeriodVal.h"

#include <boost/shared_ptr.hpp>


class PeriodValueRef {
public:
	PeriodValueRef();

	virtual double referencedVal(const PeriodVal &periodVal) const = 0;

	virtual ~PeriodValueRef();
};

typedef boost::shared_ptr<PeriodValueRef> PeriodValueRefPtr;


#endif /* PERIODVALUEREF_H_ */
