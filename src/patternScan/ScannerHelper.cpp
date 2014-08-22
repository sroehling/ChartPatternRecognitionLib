/*
 * ScannerHelper.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: sroehling
 */

#include <ScannerHelper.h>
#include "PatternMatch.h"
#include "TimeHelper.h"
#include <boost/log/trivial.hpp>
#include "PatternMatchFilter.h"

#include "SecondPeriodValuePivotsLower.h"
#include "HighestHighLessThanFirstHigh.h"
#include "LowerHighPatternMatchValidatorFactory.h"
#include "LowestLowGreaterThanLastLow.h"
#include "ValuesCloseToTrendlineValidator.h"
#include "PatternMatchValidatorCreationHelper.h"
#include "RecoverPercentOfDepth.h"


namespace scannerHelper {

void logMatchInfo(const std::string &prefix, const PatternMatch &match)
{
	BOOST_LOG_TRIVIAL(debug) << prefix
			<< ": start = " << TimeHelper::formatDate(match.firstValue().periodTime())
			<< ", end = " << TimeHelper::formatDate(match.lastValue().periodTime())
			<< ", num segments = " << match.numSegments()
			<< ", depth (pts/%) = " << match.depthPoints() << "/" << match.depthPercent()
		<< ", last close = " << match.lastValue().close();
}

void logMatchesInfo(const std::string &prefix, const PatternMatchListPtr &matches)
{
    for(PatternMatchList::const_iterator matchIter = matches->begin(); matchIter != matches->end(); matchIter++)
    {
        logMatchInfo(prefix,**matchIter);
    }
}

void appendValidatedMatches(const PatternMatchListPtr &appendTo,
		const PatternMatchListPtr &unfilteredMatches,
		const PatternMatchValidatorPtr &matchValidator)
{
	PatternMatchListPtr filteredMatches = PatternMatchValidator::filterMatches(
			matchValidator,unfilteredMatches);
	appendTo->insert(appendTo->end(),filteredMatches->begin(),filteredMatches->end());

}

void populateStandardDowntrendValidationFactories(CompositePatternMatchValidatorFactory &downTrendValidatorFactory)
{
    // After visually inspecting some pattern matches which also used the constraint SecondPeriodValuePivotsLower
    // and HighestHighLessThanFirstHigh, it was found that using ValuesCloseToTrendlineValidator by itself
    // generated a better set of pattern matches.
//    downTrendValidatorFactory.addStaticValidator(PatternMatchValidatorPtr(new LowestLowGreaterThanLastLow()));
    downTrendValidatorFactory.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
}

void populateStandardUpTrendValidationFactories(CompositePatternMatchValidatorFactory &upTrendValidatorFactory)
{
    // TODO - Uptrend needs to validate LowestLowGreaterThanFirstLow and HighestHighLessThanLastHigh
    upTrendValidatorFactory.addStaticValidator(patternMatchValidatorCreationHelper::highestHighBelowLastHigh());
    upTrendValidatorFactory.addStaticValidator(PatternMatchValidatorPtr(new ValuesCloseToTrendlineValidator()));
}

} // namespace

