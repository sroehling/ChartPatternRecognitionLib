#include "CupWithoutHandlePatternMatch.h"
#include "PatternMatchVisitor.h"

CupWithoutHandlePatternMatch::CupWithoutHandlePatternMatch(const PatternMatchPtr &cup)
    : PatternMatch(cup->segments()),
      cup_(cup)
{
    PatternMatch::initBreakoutAboveFirstHighBreakoutInfo(cup);
}

void CupWithoutHandlePatternMatch::acceptVisitor(PatternMatchVisitor &visitor)
{
    cup_->acceptVisitor(visitor);
    visitor.visitCupWithoutHandlePatternMatch(*this);
}
