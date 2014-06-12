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
public:
	EndWithinPercentOfStart();

	bool validPattern(const PatternMatch &candidateMatch);

	~EndWithinPercentOfStart();
};

#endif /* ENDWITHINPERCENTOFSTART_H_ */
