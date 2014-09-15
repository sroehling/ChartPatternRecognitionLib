/*
 * WedgePatternMatch.h
 *
 *  Created on: Aug 1, 2014
 *      Author: sroehling
 */

#ifndef SYMETRICTRIANGLEPATTERNMATCH_H_
#define SYMETRICTRIANGLEPATTERNMATCH_H_

#include <PatternMatch.h>
#include "ChartSegment.h"

#include "WedgePatternMatch.h"

class SymetricTrianglePatternMatch: public WedgePatternMatch {
public:
    SymetricTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

	virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Symetric Triangle"; }

};

#endif /* SYMETRICTRIANGLEPATTERNMATCH_H_ */
