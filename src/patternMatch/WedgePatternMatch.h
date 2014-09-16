#ifndef WEDGEPATTERNMATCH_H
#define WEDGEPATTERNMATCH_H

#include "WedgeMatchValidationInfo.h"
#include "PatternMatch.h"

class WedgePatternMatch : public PatternMatch
{
private:
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;
public:
    WedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo);

    const ChartSegmentPtr &lowerTrendLine() const { return lowerTrendLine_; }
    const ChartSegmentPtr &upperTrendLine() const { return upperTrendLine_; }

    virtual void acceptVisitor(PatternMatchVisitor &visitor);
};

#endif // WEDGEPATTERNMATCH_H
