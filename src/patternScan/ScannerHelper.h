/*
 * ScannerHelper.h
 *
 *  Created on: Jul 2, 2014
 *      Author: sroehling
 */

#ifndef SCANNERHELPER_H_
#define SCANNERHELPER_H_

#include "PatternMatch.h"

class ScannerHelper {
public:
	ScannerHelper() {}

	static void logMatchInfo(const std::string &prefix, const PatternMatch &match);


	virtual ~ScannerHelper() {}
};

#endif /* SCANNERHELPER_H_ */
