#ifndef FLATBASEPATTERNMATCH_H
#define FLATBASEPATTERNMATCH_H

#include "PatternMatch.h"
#include "WedgeMatchValidationInfo.h"

class FlatBasePatternMatch : public PatternMatch
{
private:
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;
public:
    FlatBasePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    const ChartSegmentPtr &lowerTrendLine() const { return lowerTrendLine_; }
    const ChartSegmentPtr &upperTrendLine() const { return upperTrendLine_; }

    virtual std::string matchType() const { return "Flat Base"; }

};

#endif // FLATBASEPATTERNMATCH_H
