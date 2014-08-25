#ifndef RISINGWEDGEPATTERNMATCH_H
#define RISINGWEDGEPATTERNMATCH_H

#include "WedgePatternMatch.h"

class RisingWedgePatternMatch : public WedgePatternMatch
{
public:
    RisingWedgePatternMatch(const ChartSegmentPtr &wedgeSegment,
            const ChartSegmentPtr &upperTrendLine,
            const ChartSegmentPtr &lowerTrendLine);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    virtual std::string matchType() const { return "Rising Wedge"; }
};

#endif // RISINGWEDGEPATTERNMATCH_H
