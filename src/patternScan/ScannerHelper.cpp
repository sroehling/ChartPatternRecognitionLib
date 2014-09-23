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

using namespace timeHelper;

namespace scannerHelper {

void logMatchInfo(const std::string &prefix, const PatternMatch &match)
{
	BOOST_LOG_TRIVIAL(debug) << prefix
            << ": start = " << formatDate(match.firstValue().periodTime())
            << ", end = " << formatDate(match.lastValue().periodTime())
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

} // namespace

