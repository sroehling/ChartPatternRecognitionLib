#ifndef DESCENDINGTRIANGLEPATTERNMATCH_H
#define DESCENDINGTRIANGLEPATTERNMATCH_H

#include "TrianglePatternMatch.h"

class DescendingTrianglePatternMatch : public TrianglePatternMatch
{
public:
    DescendingTrianglePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Descending Triangle"; }
};

#endif // DESCENDINGTRIANGLEPATTERNMATCH_H
