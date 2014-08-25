#ifndef FALLINGWEDGEPATTERNMATCH_H
#define FALLINGWEDGEPATTERNMATCH_H

#include "WedgePatternMatch.h"

class FallingWedgePatternMatch : public WedgePatternMatch
{
public:
    FallingWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Falling Wedge"; }
};

#endif // FALLINGWEDGEPATTERNMATCH_H
