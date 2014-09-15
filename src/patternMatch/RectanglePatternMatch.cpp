#include "RectanglePatternMatch.h"
#include "PatternMatchVisitor.h"

RectanglePatternMatch::RectanglePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo)
    : PatternMatch(wedgeMatchInfo.createWedgeSegment()),
      upperTrendLine_(wedgeMatchInfo.upperTrendLine()),
    lowerTrendLine_(wedgeMatchInfo.lowerTrendLine())
{
}


void RectanglePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    visitor.visitRectanglePatternMatch(*this);
}
