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

void ScannerHelper::logMatchInfo(const std::string &prefix, const PatternMatch &match)
{
	BOOST_LOG_TRIVIAL(debug) << prefix
			<< ": start = " << TimeHelper::formatDate(match.firstValue().periodTime())
			<< ", end = " << TimeHelper::formatDate(match.lastValue().periodTime())
			<< ", num segments = " << match.numSegments()
			<< ", depth (pts/%) = " << match.depthPoints() << "/" << match.depthPercent()
		<< ", last close = " << match.lastValue().close();
}
