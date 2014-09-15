#ifndef RECTANGLEPATTERNMATCH_H
#define RECTANGLEPATTERNMATCH_H

#include "PatternMatch.h"
#include "WedgeMatchValidationInfo.h"

class RectanglePatternMatch : public PatternMatch
{
private:
    ChartSegmentPtr upperTrendLine_;
    ChartSegmentPtr lowerTrendLine_;
public:
    RectanglePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo);

    virtual void acceptVisitor(PatternMatchVisitor &visitor);

    const ChartSegmentPtr &lowerTrendLine() const { return lowerTrendLine_; }
    const ChartSegmentPtr &upperTrendLine() const { return upperTrendLine_; }

    virtual std::string matchType() const { return "Rectangle"; }

};

#endif // RECTANGLEPATTERNMATCH_H
