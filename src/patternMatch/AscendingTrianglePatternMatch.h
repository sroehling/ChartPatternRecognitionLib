#ifndef ASCENDINGTRIANGLEPATTERNMATCH_H
#define ASCENDINGTRIANGLEPATTERNMATCH_H

#include "WedgePatternMatch.h"

class AscendingTrianglePatternMatch : public WedgePatternMatch
{
public:
    AscendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Ascending Triangle"; }
};

#endif // ASCENDINGTRIANGLEPATTERNMATCH_H
