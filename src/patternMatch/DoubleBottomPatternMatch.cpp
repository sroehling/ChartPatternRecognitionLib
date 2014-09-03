#include "DoubleBottomPatternMatch.h"
#include "PatternMatchVisitor.h"
#include "PatternMatchBreakoutInfo.h"

DoubleBottomPatternMatch::DoubleBottomPatternMatch(const PatternMatchPtr &lhsV, const PatternMatchPtr &rhsV)
    : PatternMatch(lhsV->appendMatch(*rhsV)->segments()),
      lhsV_(lhsV),rhsV_(rhsV)
{
    PatternMatch::initBreakoutAboveFirstHighBreakoutInfo(rhsV);
}


void DoubleBottomPatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    lhsV_->acceptVisitor(visitor);
    rhsV_->acceptVisitor(visitor);
    visitor.visitDoubleBottomPatternMatch(*this);
}
