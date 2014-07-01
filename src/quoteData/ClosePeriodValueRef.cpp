/*
 * ClosePeriodValueRef.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: sroehling
 */

#include <ClosePeriodValueRef.h>

ClosePeriodValueRef::ClosePeriodValueRef() {
}

double ClosePeriodValueRef::referencedVal(const PeriodVal &periodVal) const
{
	return periodVal.close();
}

ClosePeriodValueRef::~ClosePeriodValueRef() {
}

