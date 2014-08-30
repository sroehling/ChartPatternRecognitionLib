#include "FlatBasePatternMatch.h"
#include "PatternMatchVisitor.h"

FlatBasePatternMatch::FlatBasePatternMatch(const WedgeMatchValidationInfo &wedgeMatchInfo)
    : PatternMatch(wedgeMatchInfo.createWedgeSegment()),
      upperTrendLine_(wedgeMatchInfo.upperTrendLine()),
    lowerTrendLine_(wedgeMatchInfo.lowerTrendLine())
{
}


void FlatBasePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    visitor.visitFlatBasePatternMatch(*this);
}
