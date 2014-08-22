/*
 * ScannerHelper.h
 *
 *  Created on: Jul 2, 2014
 *      Author: sroehling
 */

#ifndef SCANNERHELPER_H_
#define SCANNERHELPER_H_

#include "PatternMatch.h"
#include "PatternMatchValidator.h"
#include "CompositePatternMatchValidatorFactory.h"

namespace scannerHelper {

	void logMatchInfo(const std::string &prefix, const PatternMatch &match);

    void logMatchesInfo(const std::string &prefix, const PatternMatchListPtr &matches);

	// Filter unfilteredMatches to only those pattern matches passing validation,
	// then append the results to the end of appendTo.
	void appendValidatedMatches(const PatternMatchListPtr &appendTo,
			const PatternMatchListPtr &unfilteredMatches,
			const PatternMatchValidatorPtr &matchValidator);

    void populateStandardDowntrendValidationFactories(
            CompositePatternMatchValidatorFactory &downTrendValidatorFactory);
    void populateStandardUpTrendValidationFactories(
            CompositePatternMatchValidatorFactory &upTrendValidatorFactory);

}


#endif /* SCANNERHELPER_H_ */
