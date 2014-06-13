/*
 * EndWithinPercentOfStart.h
 *
 *  Created on: Jun 10, 2014
 *      Author: sroehling
 */

#ifndef ENDWITHINPERCENTOFSTART_H_
#define ENDWITHINPERCENTOFSTART_H_

#include "PatternMatchValidator.h"
#include "PatternMatch.h"

class EndWithinPercentOfStart: public PatternMatchValidator {
private:
	double relPercent_;
	double relativePercentVal(double comparisonVal, double baseVal) const;
public:
	EndWithinPercentOfStart(double relPercent);

	bool validPattern(const PatternMatch &candidateMatch);

	~EndWithinPercentOfStart();
};

#endif /* ENDWITHINPERCENTOFSTART_H_ */
