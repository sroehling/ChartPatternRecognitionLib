#include "WedgePatternMatch.h"
#include "PatternMatchVisitor.h"

WedgePatternMatch::WedgePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo)
    : PatternMatch(wedgeMatchInfo.createWedgeSegment()),
      upperTrendLine_(wedgeMatchInfo.upperTrendLine()),
    lowerTrendLine_(wedgeMatchInfo.lowerTrendLine())
{
}


void WedgePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    visitor.visitWedgePatternMatch(*this);
}
